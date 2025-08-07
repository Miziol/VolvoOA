#include "guistyle.h"

GuiStyle::GuiStyle(SettingsManager &newSettings) : category("GUI STYLE"), appSettings(newSettings) {
    m_titleTextSize = 23;
    m_labelTextSize = 15;
    m_bodyTextSize = 10;

    /// margins
    m_margin = 10;

    /// elements
    m_detailSize = 2;
    m_smallElementSize = 30;
    m_mediumElementSize = 40;
    m_bigElementSize = 60;
}