//
// Created by fluty on 2024/2/10.
//

#ifndef CLIPEDITVIEWCONTROLLER_H
#define CLIPEDITVIEWCONTROLLER_H

#include "Model/Clip.h"
#include "Model/Note.h"
#include "Model/ClipboardDataModel/NotesParamsInfo.h"
#include "Utils/Singleton.h"

#include <QObject>

class IClipEditorView;

class ClipEditorViewController final : public QObject, public Singleton<ClipEditorViewController> {
    Q_OBJECT

public:
    void setView(IClipEditorView *view);
    void setCurrentSingingClip(SingingClip *clip);
    void copySelectedNotesWithParams() const;
    void cutSelectedNotesWithParams();
    void pasteNotesWithParams(const NotesParamsInfo &info, int tick);

    [[nodiscard]] bool canSelectAll() const;
    [[nodiscard]] bool canDelete() const;

    // View operations
    void centerAt(double tick, double keyIndex);
    void centerAt(const Note &note);

signals:
    void canSelectAllChanged(bool canSelectAll);
    void canDeleteChanged(bool canDelete); // TODO: 连接到 clip 模型监听更改

public slots:
    void onClipPropertyChanged(const Clip::ClipCommonProperties &args);
    void onRemoveNotes(const QList<int> &notesId);
    void onEditNotesLyric(const QList<int> &notesId);
    void onInsertNote(Note *note);
    void onMoveNotes(const QList<int> &notesId, int deltaTick, int deltaKey);
    void onResizeNotesLeft(const QList<int> &notesId, int deltaTick) const;
    void onResizeNotesRight(const QList<int> &notesId, int deltaTick) const;
    void onAdjustPhoneme(const QList<int> &notesId, const QList<Phoneme> &phonemes) const;
    void onNoteSelectionChanged(const QList<int> &notesId, bool unselectOther);
    void onOriginalPitchChanged(const OverlapableSerialList<Curve> &curves) const;
    void onPitchEdited(const OverlapableSerialList<Curve> &curves) const;
    void onEditSelectedNotesLyric() const;
    void onDeleteSelectedNotes();
    void onSelectAllNotes();
    void onFillLyric(QWidget *parent);

private:
    IClipEditorView *m_view = nullptr;
    SingingClip *m_clip = nullptr;

    void editNotesLyric(const QList<Note *> &notes) const;
    void removeNotes(const QList<Note *> &notes) const;
    [[nodiscard]] NotesParamsInfo buildNoteParamsInfo() const;
};



#endif // CLIPEDITVIEWCONTROLLER_H
