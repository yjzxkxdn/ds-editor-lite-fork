//
// Created by fluty on 24-9-29.
//

#ifndef MODELCHANGEHANDLER_H
#define MODELCHANGEHANDLER_H

#include "Model/AppModel/AppModel.h"
#include "Model/AppModel/SingingClip.h"
#include "Model/AppModel/Track.h"


#include <QObject>

class ModelChangeHandler : public QObject {
    Q_OBJECT

protected:
    explicit ModelChangeHandler(QObject *parent = nullptr);

    virtual void handleTrackInserted(Track *track);
    virtual void handleTrackRemoved(Track *track);
    virtual void handleClipInserted(Clip *clip);
    virtual void handleClipRemoved(Clip *clip);
    virtual void handleClipPropertyChanged(Clip *clip);
    virtual void handleSingingClipInserted(SingingClip *clip);
    virtual void handleSingingClipRemoved(SingingClip *clip);
    virtual void handleNoteChanged(SingingClip::NoteChangeType type, const QList<Note *> &notes,
                                   SingingClip *clip);
    virtual void handleParamChanged(ParamInfo::Name name, Param::Type type, SingingClip *clip);
    virtual void handlePiecesChanged(const QList<InferPiece *> &pieces, SingingClip *clip);

private slots:
    void onModelChanged();
    void onTrackChanged(AppModel::TrackChangeType type, qsizetype index, Track *track);
    void onClipChanged(Track::ClipChangeType type, Clip *clip);

private:
    QList<Track *> m_tracks;
};



#endif // MODELCHANGEHANDLER_H
