#include "OutputFormatter.h"
#include "Colors.h"
#include <iomanip>
#include <limits>
#include <sstream>

// Print main header with elegant design
void OutputFormatter::print_header(const std::string &title)
{
    std::cout << "\n";
    std::cout << Colors::SOFT_CYAN << Colors::BOLD;
    std::cout << std::string(80, '=') << "\n";
    std::cout << "  " << title << "\n";
    std::cout << std::string(80, '=') << "\n";
    std::cout << Colors::RESET;
}

// Print sub-header with softer styling
void OutputFormatter::print_sub_header(const std::string &title)
{
    std::cout << "\n"
              << Colors::SOFT_BLUE << Colors::BOLD;
    std::cout << "  " << title << "\n";
    std::cout << Colors::SOFT_GRAY << std::string(70, '-') << "\n";
    std::cout << Colors::RESET;
}

// Print section with minimal styling
void OutputFormatter::print_section(const std::string &title)
{
    std::cout << "\n"
              << Colors::SOFT_PURPLE << "  > " << Colors::BOLD
              << title << Colors::RESET << "\n";
}

// Print section title with category styling
void OutputFormatter::print_section_title(const std::string &title)
{
    std::cout << "\n"
              << Colors::SOFT_YELLOW << Colors::BOLD
              << "  " << title << Colors::RESET << "\n";
}

// Print simple line
void OutputFormatter::print_line(int width)
{
    std::cout << Colors::SOFT_GRAY << std::string(width, '-') << Colors::RESET << "\n";
}

// Print double line
void OutputFormatter::print_double_line(int width)
{
    std::cout << Colors::SOFT_CYAN << std::string(width, '=') << Colors::RESET << "\n";
}

// Print dotted line for subtle separation
void OutputFormatter::print_dotted_line(int width)
{
    std::cout << Colors::LIGHT_GRAY;
    for (int i = 0; i < width; i += 2)
    {
        std::cout << ". ";
    }
    std::cout << Colors::RESET << "\n";
}

// Print elegant separator
void OutputFormatter::print_elegant_separator()
{
    std::cout << "\n"
              << Colors::SOFT_GRAY << "  "
              << std::string(76, '-') << Colors::RESET << "\n\n";
}

// Print box header with border
void OutputFormatter::print_box_header(const std::string &title, int width)
{
    std::cout << "\n"
              << Colors::SOFT_CYAN;
    std::cout << "  " << std::string(width - 4, '-') << "\n";
    std::cout << "  | " << Colors::BOLD << title << Colors::RESET
              << Colors::SOFT_CYAN << std::string(width - title.length() - 7, ' ') << "|\n";
    std::cout << "  " << std::string(width - 4, '-') << Colors::RESET << "\n";
}

// Print info card (label-value pair with styling)
void OutputFormatter::print_info_card(const std::string &label, const std::string &value)
{
    std::cout << "  " << Colors::SOFT_GRAY << label << ": "
              << Colors::RESET << Colors::BOLD << value << Colors::RESET << "\n";
}

// Print data row with fixed label width
void OutputFormatter::print_data_row(const std::string &label, const std::string &value, int labelWidth)
{
    std::cout << "  " << Colors::SOFT_GRAY << std::left << std::setw(labelWidth)
              << label << Colors::RESET << " " << Colors::BOLD << value
              << Colors::RESET << "\n";
}

// Get colored status indicator
std::string OutputFormatter::get_status_indicator(bool active)
{
    if (active)
    {
        return Colors::SOFT_GREEN + "[ACTIVE]" + Colors::RESET;
    }
    else
    {
        return Colors::SOFT_RED + "[INACTIVE]" + Colors::RESET;
    }
}

// Get colored status based on text
std::string OutputFormatter::get_colored_status(const std::string &status)
{
    if (status == "SUCCESS" || status == "APPROVED" || status == "PASSED")
    {
        return Colors::SOFT_GREEN + status + Colors::RESET;
    }
    else if (status == "FAILED" || status == "REJECTED" || status == "ERROR")
    {
        return Colors::SOFT_RED + status + Colors::RESET;
    }
    else if (status == "PENDING" || status == "WAITING")
    {
        return Colors::SOFT_YELLOW + status + Colors::RESET;
    }
    else
    {
        return Colors::SOFT_GRAY + status + Colors::RESET;
    }
}

// Get progress bar with elegant styling
std::string OutputFormatter::get_progress_bar(double percentage, int width)
{
    int filled = static_cast<int>(percentage / 100.0 * width);

    std::string bar = Colors::SOFT_CYAN + "[";
    for (int i = 0; i < width; i++)
    {
        if (i < filled)
        {
            bar += "=";
        }
        else
        {
            bar += " ";
        }
    }
    bar += "] " + Colors::SOFT_BLUE + std::to_string(static_cast<int>(percentage)) + "%" + Colors::RESET;

    return bar;
}

// Get badge with color
std::string OutputFormatter::get_badge(const std::string &text, const std::string &color)
{
    return color + "[" + text + "]" + Colors::RESET;
}

// Print table header
void OutputFormatter::print_table_header(const std::vector<std::string> &headers,
                                         const std::vector<int> &widths)
{
    std::cout << "  " << Colors::SOFT_CYAN << Colors::BOLD;
    for (size_t i = 0; i < headers.size(); ++i)
    {
        std::cout << std::left << std::setw(widths[i]) << headers[i];
        if (i < headers.size() - 1)
            std::cout << " | ";
    }
    std::cout << Colors::RESET << "\n";
}

// Print table row
void OutputFormatter::print_table_row(const std::vector<std::string> &cells,
                                      const std::vector<int> &widths)
{
    std::cout << "  ";
    for (size_t i = 0; i < cells.size(); ++i)
    {
        std::cout << std::left << std::setw(widths[i]) << cells[i];
        if (i < cells.size() - 1)
            std::cout << Colors::SOFT_GRAY << " | " << Colors::RESET;
    }
    std::cout << "\n";
}

// Print table separator
void OutputFormatter::print_table_separator(const std::vector<int> &widths)
{
    std::cout << "  " << Colors::SOFT_GRAY;
    int totalWidth = 0;
    for (size_t i = 0; i < widths.size(); ++i)
    {
        totalWidth += widths[i];
        if (i < widths.size() - 1)
            totalWidth += 3; // for " | "
    }
    std::cout << std::string(totalWidth, '-') << Colors::RESET << "\n";
}

// Pause with elegant message
void OutputFormatter::pause()
{
    std::cout << "\n"
              << Colors::SOFT_GRAY << "  Press "
              << Colors::BOLD << "[Enter]" << Colors::RESET
              << Colors::SOFT_GRAY << " to continue..." << Colors::RESET;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin.get();
}

// Pause and clear screen
void OutputFormatter::pause_and_clear()
{
    pause();
    clear_screen();
}

// Wait for enter with custom message
void OutputFormatter::wait_for_enter(const std::string &message)
{
    if (!message.empty())
    {
        std::cout << "\n"
                  << Colors::SOFT_GRAY << "  " << message << Colors::RESET;
    }
    else
    {
        pause();
        return;
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin.get();
}

// Print menu item with elegant styling
void OutputFormatter::print_menu_item(int number, const std::string &text, const std::string &color)
{
    std::string itemColor = color.empty() ? Colors::SOFT_GREEN : color;

    std::cout << "    " << itemColor;

    // Always use consistent width: [XX] for all numbers
    if (number < 10)
    {
        std::cout << "[ " << number << "]";
    }
    else
    {
        std::cout << "[" << number << "]";
    }

    std::cout << Colors::RESET << "  " << text << "\n";
}

// Print menu separator
void OutputFormatter::print_menu_separator()
{
    std::cout << "\n";
}

// Print menu category
void OutputFormatter::print_menu_category(const std::string &category)
{
    std::cout << "\n"
              << Colors::SOFT_YELLOW << Colors::BOLD
              << "  " << category << Colors::RESET << "\n";
}

// Print success message
void OutputFormatter::print_success(const std::string &message)
{
    std::cout << "\n  " << Colors::SOFT_GREEN << Colors::BOLD << "[SUCCESS] "
              << Colors::RESET << Colors::SOFT_GREEN << message << Colors::RESET << "\n";
}

// Print error message
void OutputFormatter::print_error(const std::string &message)
{
    std::cout << "\n  " << Colors::SOFT_RED << Colors::BOLD << "[ERROR] "
              << Colors::RESET << Colors::SOFT_RED << message << Colors::RESET << "\n";
}

// Print warning message
void OutputFormatter::print_warning(const std::string &message)
{
    std::cout << "\n  " << Colors::SOFT_ORANGE << Colors::BOLD << "[WARNING] "
              << Colors::RESET << Colors::SOFT_ORANGE << message << Colors::RESET << "\n";
}

// Print info message
void OutputFormatter::print_info(const std::string &message)
{
    std::cout << "\n  " << Colors::SOFT_BLUE << Colors::BOLD << "[INFO] "
              << Colors::RESET << Colors::SOFT_BLUE << message << Colors::RESET << "\n";
}

// Print centered text
void OutputFormatter::print_centered(const std::string &text, int width)
{
    int padding = (width - text.length()) / 2;
    if (padding > 0)
    {
        std::cout << std::string(padding, ' ');
    }
    std::cout << text << "\n";
}

// Clear screen
void OutputFormatter::clear_screen()
{
    std::cout << "\033[2J\033[H"; // ANSI escape codes
}
