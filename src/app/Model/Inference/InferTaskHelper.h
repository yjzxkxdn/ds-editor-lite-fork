//
// Created by fluty on 24-10-2.
//

#ifndef INFERTASKHELPER_H
#define INFERTASKHELPER_H
#include <QList>

class InferDurPitNote;
class InferWord;

class InferTaskHelper {
public:
    static QList<InferWord> buildWords(const QList<InferDurPitNote> &notes, double tempo,
                                       bool useOffsetInfo = false);
};



#endif // INFERTASKHELPER_H
