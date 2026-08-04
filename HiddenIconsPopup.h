#ifndef HIDDENICONSPOPUP_H
#define HIDDENICONSPOPUP_H
#pragma once
#include <QDialog>
#include <QGridLayout>
#include <QCheckBox>
#include <QPainter>
#include <QPainterPath>
#include <QEvent>
#include <QPropertyAnimation> // CRITICAL NEW INCLUDE

class HiddenIconsPopup : public QDialog {
    Q_OBJECT
public:
    explicit HiddenIconsPopup(QWidget *parent = nullptr) : QDialog(parent) {
        setWindowFlags(Qt::Popup | Qt::FramelessWindowHint | Qt::NoDropShadowWindowHint);
        setAttribute(Qt::WA_TranslucentBackground);
        setAttribute(Qt::WA_DeleteOnClose);

        // Start completely transparent for the fade animation
        setWindowOpacity(0.0);

        QGridLayout *gridLayout = new QGridLayout(this);
        gridLayout->setContentsMargins(12, 12, 22, 12);
        gridLayout->setSpacing(8);

        setStyleSheet(
            "QCheckBox { color: white; padding: 5px; }"
            "QCheckBox::hover { background-color: #333333; border-radius: 4px; }"
            );

        QCheckBox *cb1 = new QCheckBox("Item 1", this);
        QCheckBox *cb2 = new QCheckBox("Item 2", this);
        QCheckBox *cb3 = new QCheckBox("Item 3", this);

        gridLayout->addWidget(cb1, 0, 0);
        gridLayout->addWidget(cb2, 0, 1);
        gridLayout->addWidget(cb3, 1, 0);

        resize(220, 120);
    }

    // THE ANIMATION TRIGGER FUNCTION
    void startFadeIn(int durationMs = 300) {
        // Show the window framework on screen first
        this->show();

        // Create animation target mapping the window's opacity property
        QPropertyAnimation *animation = new QPropertyAnimation(this, "windowOpacity");
        animation->setDuration(durationMs);      // Set speed (e.g., 300ms)
        animation->setStartValue(0.0);           // Invisible
        animation->setEndValue(1.0);             // Fully visible
        animation->setEasingCurve(QEasingCurve::OutCubic); // Smooth slowing effect near the end

        // Start animation and automatically clean up its memory when finished
        animation->start(QAbstractAnimation::DeleteWhenStopped);
    }

protected:
    void paintEvent(QPaintEvent *event) override {
        Q_UNUSED(event);
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing);

        QColor bgColor("#242424");
        QColor borderColor("#3d3d3d");

        int arrowWidth = 10;
        int arrowHeight = 16;
        int cornerRadius = 6;

        QRect bodyRect(0, 0, width() - arrowWidth, height());
        QPainterPath path;

        int arrowY = height() / 2;
        int arrowXBase = width() - arrowWidth;

        path.moveTo(arrowXBase, arrowY - (arrowHeight / 2));
        path.lineTo(width(), arrowY);
        path.lineTo(arrowXBase, arrowY + (arrowHeight / 2));

        path.addRoundedRect(bodyRect, cornerRadius, cornerRadius);

        painter.fillPath(path, bgColor);
        QPen pen(borderColor, 1);
        painter.setPen(pen);
        painter.drawPath(path);
    }

    void changeEvent(QEvent *event) override {
        if (event->type() == QEvent::ActivationChange && !isActiveWindow()) {
            close();
        }
        QDialog::changeEvent(event);
    }
};




#endif // HIDDENICONSPOPUP_H
