#ifndef HIDDENICONSPOPUP_H
#define HIDDENICONSPOPUP_H
#pragma once

#include <QDialog>
#include <QGridLayout>
#include <QCheckBox>
#include <QPainter>
#include <QPainterPath>
#include <QEvent>
#include <QPropertyAnimation>
#include <QStringList> // Better suited for lists of strings in Qt

class HiddenIconsPopup : public QDialog {
    Q_OBJECT
private:
    QGridLayout *gridLayout; // Move layout to private member

public:
    explicit HiddenIconsPopup(QWidget *parent = nullptr) : QDialog(parent) {
        setWindowFlags(Qt::Popup | Qt::FramelessWindowHint | Qt::NoDropShadowWindowHint);
        setAttribute(Qt::WA_TranslucentBackground);
        setAttribute(Qt::WA_DeleteOnClose);
        setWindowOpacity(0.0);

        setStyleSheet(
            "QCheckBox { color: white; padding: 5px; }"
            "QCheckBox::hover { background-color: #333333; border-radius: 4px; }"
            );

        // Initialize the central layout exactly once
        gridLayout = new QGridLayout(this);
        gridLayout->setContentsMargins(12, 12, 22, 12); // Extra 22px margin on right for the arrow
        gridLayout->setSpacing(8);
    }

    // Changed to QStringList (safer and cleaner for Qt APIs)
    void addPorts(const QStringList &ports) {
        // Clear previous widgets if this function is called multiple times
        QLayoutItem *child;
        while ((child = gridLayout->takeAt(0)) != nullptr) {
            if (child->widget()) {
                child->widget()->deleteLater();
            }
            delete child;
        }

        const int maxColumns = 2; // Change this to 3 or 4 if you want wider grids

        for (int i = 0; i < ports.size(); ++i) {
            QCheckBox *cb = new QCheckBox(ports.at(i), this);

            // Math trick to wrap layout into structured columns
            int row = i / maxColumns;
            int col = i % maxColumns;

            gridLayout->addWidget(cb, row, col);
        }

        // Force layout calculations and resize window automatically to fit items
        gridLayout->activate();
        adjustSize();

        // Add padding to height/width for custom drawn borders & side arrow
        resize(width() + 15, height());
    }

    void startFadeIn(int durationMs = 300) {
        this->show();
        QPropertyAnimation *animation = new QPropertyAnimation(this, "windowOpacity");
        animation->setDuration(durationMs);
        animation->setStartValue(0.0);
        animation->setEndValue(1.0);
        animation->setEasingCurve(QEasingCurve::OutCubic);
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

        // Ensure painting matches the dynamic window size
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
