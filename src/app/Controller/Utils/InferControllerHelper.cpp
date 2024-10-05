//
// Created by fluty on 24-9-6.
//

#include "InferControllerHelper.h"

#include "Model/AppModel/DrawCurve.h"
#include "Model/AppModel/Note.h"
#include "Model/AppModel/SingingClip.h"
#include "Model/Inference/InferInputNote.h"
#include "Model/Inference/InferPiece.h"
#include "Model/Inference/PhonemeNameModel.h"
#include "Utils/Linq.h"

#include <QDebug>

void InferControllerHelper::updatePronunciation(const QList<Note *> &notes,
                                                const QList<QString> &args, SingingClip &clip) {
    if (notes.count() != args.count()) {
        qFatal() << "updateNotesPronunciation() note count != args count:" << notes.count()
                 << args.count();
        return;
    }
    int i = 0;
    for (const auto note : notes) {
        note->setPronunciation(Note::Original, args[i]);
        i++;
    }
    clip.notifyNoteChanged(SingingClip::OriginalWordPropertyChange, notes);
}

void InferControllerHelper::updatePhoneName(const QList<Note *> &notes,
                                            const QList<PhonemeNameResult> &args,
                                            SingingClip &clip) {
    if (notes.count() != args.count()) {
        qFatal() << "updateNotesPhonemeName() note count != args count:" << notes.count()
                 << args.count();
        return;
    }
    int i = 0;
    for (const auto note : notes) {
        note->setPhonemeNameInfo(Phonemes::Ahead, Note::Original, args[i].aheadNames);
        note->setPhonemeNameInfo(Phonemes::Normal, Note::Original, args[i].normalNames);
        i++;
    }
    clip.notifyNoteChanged(SingingClip::OriginalWordPropertyChange, notes);
}

void InferControllerHelper::updatePhoneOffset(const QList<Note *> &notes,
                                              const QList<InferInputNote> &args,
                                              SingingClip &clip) {
    if (notes.count() != args.count()) {
        qFatal() << "updateNotesPhonemeName() note count != args count:" << notes.count()
                 << args.count();
        return;
    }
    int i = 0;
    for (const auto note : notes) {
        note->setPhonemeOffsetInfo(Phonemes::Ahead, Note::Original, args[i].aheadOffsets);
        note->setPhonemeOffsetInfo(Phonemes::Normal, Note::Original, args[i].normalOffsets);
        i++;
    }
    clip.notifyNoteChanged(SingingClip::OriginalWordPropertyChange, notes);
}

void InferControllerHelper::resetPhoneOffset(const QList<Note *> &notes, SingingClip &clip) {
    for (const auto note : notes) {
        note->setPhonemeOffsetInfo(Phonemes::Ahead, Note::Original, {});
        note->setPhonemeOffsetInfo(Phonemes::Normal, Note::Original, {});
    }
    clip.notifyNoteChanged(SingingClip::OriginalWordPropertyChange, notes);
}

void InferControllerHelper::resetPieceParam(ParamInfo::Name name, InferPiece &piece) {
    DrawCurve emptyCurve;
    piece.setCurve(name, emptyCurve);
    piece.clip->updateOriginalParam(name);
}

// 可能要传入转换方法，而不是直接 * 100（音高参数）
void InferControllerHelper::updateParam(const ParamInfo::Name name,
                                        const InferParamCurve &taskResult, SingingClip &clip,
                                        InferPiece &piece) {
    // 将推理结果保存到分段内部
    DrawCurve resultCurve;
    resultCurve.start = piece.realStartTick();
    resultCurve.setValues(
        Linq::selectMany(taskResult.values, L_PRED(v, static_cast<int>(v * 100))));
    piece.setCurve(name, resultCurve);
    clip.updateOriginalParam(name);
}