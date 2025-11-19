#include "Colors.h"

namespace Colors
{
    // Reset and formatting
    const std::string RESET = "\033[0m";
    const std::string BOLD = "\033[1m";
    const std::string DIM = "\033[2m";
    const std::string UNDERLINE = "\033[4m";
    const std::string ITALIC = "\033[3m";

    // Soft pastel color palette (using 256-color mode for better aesthetics)
    const std::string SOFT_CYAN = "\033[38;5;117m";   // Sky Blue - Headers
    const std::string SOFT_BLUE = "\033[38;5;75m";    // Cornflower Blue - Important info
    const std::string SOFT_GREEN = "\033[38;5;114m";  // Light Green - Success
    const std::string SOFT_YELLOW = "\033[38;5;228m"; // Khaki - Warnings
    const std::string SOFT_ORANGE = "\033[38;5;215m"; // Pastel Orange - Alerts
    const std::string SOFT_RED = "\033[38;5;210m";    // Light Coral - Errors
    const std::string SOFT_PURPLE = "\033[38;5;183m"; // Lavender - Highlights
    const std::string SOFT_PINK = "\033[38;5;218m";   // Light Pink - Accent
    const std::string SOFT_GRAY = "\033[38;5;248m";   // Dark Gray - Secondary
    const std::string LIGHT_GRAY = "\033[38;5;252m";  // Light Gray - Subtle

    // Neutral tones
    const std::string WARM_WHITE = "\033[38;5;255m"; // Off-white
    const std::string CHARCOAL = "\033[38;5;240m";   // Dark gray

    // Legacy foreground colors (for backward compatibility)
    const std::string BLACK = "\033[30m";
    const std::string RED = "\033[31m";
    const std::string GREEN = "\033[32m";
    const std::string YELLOW = "\033[33m";
    const std::string BLUE = "\033[34m";
    const std::string MAGENTA = "\033[35m";
    const std::string CYAN = "\033[36m";
    const std::string WHITE = "\033[37m";

    // Bright variants (legacy)
    const std::string BRIGHT_BLACK = "\033[90m";
    const std::string BRIGHT_RED = "\033[91m";
    const std::string BRIGHT_GREEN = "\033[92m";
    const std::string BRIGHT_YELLOW = "\033[93m";
    const std::string BRIGHT_BLUE = "\033[94m";
    const std::string BRIGHT_MAGENTA = "\033[95m";
    const std::string BRIGHT_CYAN = "\033[96m";
    const std::string BRIGHT_WHITE = "\033[97m";
}
