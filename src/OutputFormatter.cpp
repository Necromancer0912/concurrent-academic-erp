#include "OutputFormatter.h"
#include "Colors.h"
#include <iomanip>
#include <limits>

// print main header (full width with double equals)
void OutputFormatter::print_header(const std::string &title)
{
    std::cout << Colors::BOLD << Colors::CYAN;
    std::cout << "\n";
    std::cout << std::string(80, '=') << "\n";
    std::cout << "         " << title << "\n";
    std::cout << std::string(80, '=') << "\n";
    std::cout << Colors::RESET;
}

// print sub-header (section divider with arrow)
void OutputFormatter::print_sub_header(const std::string &title)
{
    std::cout << "\n"
              << Colors::YELLOW << Colors::BOLD;
    std::cout << "> " << title << "\n";
    std::cout << Colors::DIM << std::string(60, '-') << "\n";
    std::cout << Colors::RESET;
}

// print section (minimal with bullet)
void OutputFormatter::print_section(const std::string &title)
{
    std::cout << "\n"
              << Colors::CYAN << "■ " << Colors::BOLD
              << title << Colors::RESET << "\n";
}

// print simple line
void OutputFormatter::print_line(int width)
{
    std::cout << Colors::DIM << std::string(width, '-') << Colors::RESET << "\n";
}

// print double line
void OutputFormatter::print_double_line(int width)
{
    std::cout << Colors::DIM << std::string(width, '=') << Colors::RESET << "\n";
}

// get colored status indicator
std::string OutputFormatter::get_status_indicator(bool active)
{
    if (active)
    {
        return Colors::GREEN + "●" + Colors::RESET;
    }
    else
    {
        return Colors::RED + "●" + Colors::RESET;
    }
}

// get progress bar
std::string OutputFormatter::get_progress_bar(double percentage, int width)
{
    int filled = static_cast<int>(percentage / 100.0 * width);

    std::string bar = "[";
    for (int i = 0; i < width; i++)
    {
        if (i < filled)
        {
            bar += "█";
        }
        else
        {
            bar += "░";
        }
    }
    bar += "] " + std::to_string(static_cast<int>(percentage)) + "%";

    return bar;
}

// pause with message
void OutputFormatter::pause()
{
    std::cout << "\n"
              << Colors::DIM
              << Colors::DIM << "Press " << Colors::BOLD << "[Enter]" << Colors::RESET
              << Colors::DIM << " to continue" << Colors::RESET
              << Colors::RESET;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin.get();
}

// pause and clear screen
void OutputFormatter::pause_and_clear()
{
    pause();
    std::cout << "\033[2J\033[H"; // clear screen
}

// print menu item with color
void OutputFormatter::print_menu_item(int number, const std::string &text, const std::string &color)
{
    std::string item_color = color.empty() ? Colors::GREEN : color;

    std::cout << "  " << item_color;

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

// print menu separator
void OutputFormatter::print_menu_separator()
{
    std::cout << "\n"
              << Colors::DIM << "  " << std::string(60, '-') << Colors::RESET << "\n\n";
}
