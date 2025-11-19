#ifndef COLORS_H
#define COLORS_H

#include <string>

namespace Colors
{
    // Reset and formatting
    extern const std::string RESET;
    extern const std::string BOLD;
    extern const std::string DIM;
    extern const std::string UNDERLINE;
    extern const std::string ITALIC;

    // Soft pastel color palette for better aesthetics
    extern const std::string SOFT_CYAN;   // #87CEEB (Sky Blue) - Headers, titles
    extern const std::string SOFT_BLUE;   // #6495ED (Cornflower Blue) - Important info
    extern const std::string SOFT_GREEN;  // #90EE90 (Light Green) - Success, positive
    extern const std::string SOFT_YELLOW; // #F0E68C (Khaki) - Warnings, sections
    extern const std::string SOFT_ORANGE; // #FFB347 (Pastel Orange) - Alerts
    extern const std::string SOFT_RED;    // #FF6B6B (Light Coral) - Errors
    extern const std::string SOFT_PURPLE; // #B19CD9 (Lavender) - Special highlights
    extern const std::string SOFT_PINK;   // #FFB6C1 (Light Pink) - Accent
    extern const std::string SOFT_GRAY;   // #A9A9A9 (Dark Gray) - Secondary text
    extern const std::string LIGHT_GRAY;  // #D3D3D3 (Light Gray) - Subtle elements

    // Neutral tones
    extern const std::string WARM_WHITE; // Off-white for text
    extern const std::string CHARCOAL;   // Dark gray for emphasis

    // Legacy colors (for backward compatibility)
    extern const std::string BLACK;
    extern const std::string RED;
    extern const std::string GREEN;
    extern const std::string YELLOW;
    extern const std::string BLUE;
    extern const std::string MAGENTA;
    extern const std::string CYAN;
    extern const std::string WHITE;

    // Bright variants (legacy)
    extern const std::string BRIGHT_BLACK;
    extern const std::string BRIGHT_RED;
    extern const std::string BRIGHT_GREEN;
    extern const std::string BRIGHT_YELLOW;
    extern const std::string BRIGHT_BLUE;
    extern const std::string BRIGHT_MAGENTA;
    extern const std::string BRIGHT_CYAN;
    extern const std::string BRIGHT_WHITE;
}

#endif // COLORS_H
