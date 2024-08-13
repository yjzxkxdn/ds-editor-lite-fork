//
// Created by fluty on 2024/1/22.
//

#ifndef SINGINGCLIPGRAPHICSITEM_H
#define SINGINGCLIPGRAPHICSITEM_H

#include "AbstractClipView.h"
#include "Model/AppModel/Clip.h"
#include "Utils/OverlappableSerialList.h"

class Note;

class SingingClipView final : public AbstractClipView {
    Q_OBJECT
public:
    class NoteViewModel : public Overlappable {
    public:
        int id{};
        int rStart{};
        int length{};
        int keyIndex{};

        int compareTo(const NoteViewModel *obj) const;
        static bool isOverlappedWith(NoteViewModel *obj);
        [[nodiscard]] std::tuple<qsizetype, qsizetype> interval() const override;
    };

    [[nodiscard]] ClipType clipType() const override {
        return Singing;
    }
    explicit SingingClipView(int itemId, QGraphicsItem *parent = nullptr);
    ~SingingClipView() override = default;

    void loadNotes(const OverlappableSerialList<Note> &notes);

public slots:
    void onNoteListChanged(SingingClip::NoteChangeType type, Note *note);
    void onNotePropertyChanged(Note::NotePropertyType type, Note *note);
    void setDefaultLanguage(AppGlobal::LanguageType language);

private:
    // void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
    // override;
    QString text() const override;
    void drawPreviewArea(QPainter *painter, const QRectF &previewRect, int opacity) override;
    QString clipTypeName() const override;

    QList<NoteViewModel *> m_notes;
    AppGlobal::LanguageType m_language = AppGlobal::unknown;

    void addNote(Note *note);
    void removeNote(int id);
    // void updateNote(Note *note);
};



#endif // SINGINGCLIPGRAPHICSITEM_H
