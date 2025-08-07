#ifndef AP600_GUI_GUISTYLE_H
#define AP600_GUI_GUISTYLE_H

#include <QObject>

#include "../logging/loggingCategory.h"
#include "settingsManager.h"

class GuiStyle : public QObject {
    Q_OBJECT

signals:
    void textSizeChanged();
    void sizeChanged();

public:
    GuiStyle(SettingsManager &newSettings);

public:
    // text size
    Q_PROPERTY(int titleTextSize MEMBER m_titleTextSize NOTIFY textSizeChanged);
    Q_PROPERTY(int labelTextSize MEMBER m_labelTextSize NOTIFY textSizeChanged);
    Q_PROPERTY(int bodyTextSize MEMBER m_bodyTextSize NOTIFY textSizeChanged);

    /// Margins
    Q_PROPERTY(int margin MEMBER m_margin NOTIFY sizeChanged);

    /// Elements
    Q_PROPERTY(int detailSize MEMBER m_detailSize NOTIFY sizeChanged);
    Q_PROPERTY(int smallElementSize MEMBER m_smallElementSize NOTIFY sizeChanged);
    Q_PROPERTY(int mediumElementSize MEMBER m_mediumElementSize NOTIFY sizeChanged);
    Q_PROPERTY(int bigElementSize MEMBER m_bigElementSize NOTIFY sizeChanged);

private:
    QLoggingCategory category;
    SettingsManager &appSettings;

    int m_titleTextSize;
    int m_labelTextSize;
    int m_bodyTextSize;

    int m_margin;

    int m_detailSize;
    int m_smallElementSize;
    int m_mediumElementSize;
    int m_bigElementSize;
};

#endif  // AP600_GUI_GUISTYLE_H