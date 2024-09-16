//
// Created by fluty on 2024/2/10.
//

#include "ClipEditorView.h"

#include "PhonemeView.h"
#include "Controller/AppController.h"
#include "Controller/ClipEditorViewController.h"
#include "Controller/TracksViewController.h"
#include "Model/AppModel/AppModel.h"
#include "Model/AppStatus/AppStatus.h"
#include "ParamEditor/ParamEditorView.h"
#include "PianoRoll/PianoRollGraphicsView.h"
#include "PianoRoll/PianoRollView.h"
#include "ToolBar/ClipEditorToolBarView.h"

#include <QLabel>
#include <QMouseEvent>
#include <QSplitter>
#include <QVBoxLayout>

ClipEditorView::ClipEditorView(QWidget *parent) : PanelView(AppGlobal::ClipEditor, parent) {
    setAttribute(Qt::WA_StyledBackground);
    setObjectName("ClipEditorView");

    m_toolbarView = new ClipEditorToolBarView;
    m_toolbarView->setVisible(false);

    m_pianoRollEditorView = new PianoRollEditorView;

    auto mainLayout = new QVBoxLayout;
    mainLayout->addWidget(m_toolbarView);
    mainLayout->addWidget(m_pianoRollEditorView);
    mainLayout->setSpacing(0);
    mainLayout->setContentsMargins({1, 1, 1, 1});
    setLayout(mainLayout);

    clipController->setView(this);
    appController->registerPanel(this);
    installEventFilter(this);

    connect(m_toolbarView, &ClipEditorToolBarView::editModeChanged, m_pianoRollEditorView->pianoRollView(),
            &PianoRollView::onEditModeChanged);

    connect(appModel, &AppModel::modelChanged, this, &ClipEditorView::onModelChanged);
    connect(appStatus, &AppStatus::activeClipIdChanged, this, &ClipEditorView::onActiveClipChanged);
}

void ClipEditorView::centerAt(double tick, double keyIndex) {
    m_pianoRollEditorView->pianoRollView()->graphicsView()->setViewportCenterAt(tick, keyIndex);
}

void ClipEditorView::centerAt(double startTick, double length, double keyIndex) {
    auto centerTick = startTick + length / 2;
    m_pianoRollEditorView->pianoRollView()->graphicsView()->setViewportCenterAt(centerTick, keyIndex);
}

void ClipEditorView::onModelChanged() {
    m_toolbarView->setDataContext(nullptr);
    reset();
}

void ClipEditorView::onActiveClipChanged(int clipId) const {
    auto clip = appModel->findClipById(clipId);
    m_toolbarView->setDataContext(clip);
    clipController->setClip(clip);

    if (clip == nullptr)
        moveToNullClipState();
    else if (clip->clipType() == Clip::Singing)
        moveToSingingClipState(dynamic_cast<SingingClip *>(clip));
    else if (clip->clipType() == Clip::Audio)
        moveToAudioClipState(nullptr);
}

bool ClipEditorView::eventFilter(QObject *watched, QEvent *event) {
    if (event->type() == QMouseEvent::MouseButtonPress)
        appController->setActivePanel(AppGlobal::ClipEditor);
    return QWidget::eventFilter(watched, event);
}

void ClipEditorView::moveToSingingClipState(SingingClip *clip) const {
    m_toolbarView->setVisible(true);
    m_pianoRollEditorView->setVisible(true);
    m_pianoRollEditorView->setDataContext(clip);
    m_pianoRollEditorView->pianoRollView()->onEditModeChanged(m_toolbarView->editMode());
}

void ClipEditorView::moveToAudioClipState(AudioClip *clip) const {
    Q_UNUSED(clip);
    m_toolbarView->setVisible(true);
    m_pianoRollEditorView->setVisible(false);
    m_pianoRollEditorView->setDataContext(nullptr);
}

void ClipEditorView::moveToNullClipState() const {
    m_toolbarView->setVisible(false);
    m_pianoRollEditorView->setVisible(false);
    m_pianoRollEditorView->setDataContext(nullptr);
}

void ClipEditorView::reset() const {
    onActiveClipChanged(-1);
}
