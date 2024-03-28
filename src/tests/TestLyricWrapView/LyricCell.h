#ifndef LYRICCELL_H
#define LYRICCELL_H

#include <QObject>
#include <QPainter>

#include <LangCommon.h>
#include <QGraphicsScene>
#include <QGraphicsProxyWidget>

#include <QApplication>

#include "LyricLineEdit.h"

namespace LyricWrap {
    class LyricCell final : public QGraphicsObject {
        Q_OBJECT
    public:
        explicit LyricCell(const qreal &x, const qreal &y, LangNote *note,
                           QGraphicsItem *parent = nullptr);
        ~LyricCell() override;

        [[nodiscard]] QString lyric() const;
        [[nodiscard]] QString syllable() const;

        void setFont(const QFont &font);
        void setLyricRect(const QRect &rect);
        void setSyllableRect(const QRect &rect);

        [[nodiscard]] qreal width() const;
        [[nodiscard]] qreal height() const;

        void setLyric(const QString &lyric) const;

    Q_SIGNALS:
        void updateLyricSignal() const;
        void updateWidthSignal(const qreal &w) const;

    protected:
        void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;

        void contextMenuEvent(QGraphicsSceneContextMenuEvent *event) override;

        void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                   QWidget *widget) override;

        [[nodiscard]] QRectF boundingRect() const override;

    private:
        enum State {
            Normal = 0,
            Hovered = 1,
            Selected = 2,
        };

        QBrush m_backgroundBrush[3] = {QColor(155, 186, 255), QColor(169, 196, 255),
                                       QColor(169, 196, 255)};
        QPen m_borderPen[3] = {QPen(QColor(112, 156, 255), 2), QPen(QColor(112, 156, 255), 2),
                               QPen(QColor(Qt::white), 2)};
        QPen m_lyricPen[3] = {QColor(Qt::black), QColor(Qt::black), QColor(Qt::black)};
        QPen m_syllablePen[3] = {QColor(Qt::white), QColor(Qt::white), QColor(Qt::white)};

        [[nodiscard]] qreal syllableWidth() const;
        [[nodiscard]] qreal lyricWidth() const;

        [[nodiscard]] QPointF syllablePos() const;
        [[nodiscard]] QPointF lyricPos() const;
        [[nodiscard]] QRectF lyricRect() const;
        [[nodiscard]] QPointF rectPos() const;

        void updateLyric();

        QRect m_lRect;
        QRect m_sRect;

        LangNote *m_note;

        qreal m_lsPadding = 5;
        qreal m_rectPadding = 3;
        qreal m_padding = 5;
        qreal m_reckBorder = 2.5;

        QString mText;
        QFont m_font = QApplication::font();

        bool isLyricEditing = false;
        QPointer<LyricLineEdit> m_lyricEdit = new LyricLineEdit();
        QPointer<QGraphicsProxyWidget> m_lyricWidget = new QGraphicsProxyWidget();

        qreal m_width{};
        qreal m_height{};
    };
}

#endif // LYRICCELL_H