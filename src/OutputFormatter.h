#ifndef OUTPUT_FORMATTER_H
#define OUTPUT_FORMATTER_H

#include <string>
#include <iostream>
#include <vector>

class OutputFormatter
{
public:
    // Print formatted headers with elegant styling
    static void print_header(const std::string &title);
    static void print_sub_header(const std::string &title);
    static void print_section(const std::string &title);
    static void print_section_title(const std::string &title);

    // Print decorative lines
    static void print_line(int width = 80);
    static void print_double_line(int width = 80);
    static void print_dotted_line(int width = 80);
    static void print_elegant_separator();

    // Box and card designs
    static void print_box_header(const std::string &title, int width = 80);
    static void print_info_card(const std::string &label, const std::string &value);
    static void print_data_row(const std::string &label, const std::string &value, int labelWidth = 25);

    // Status indicators with soft colors
    static std::string get_status_indicator(bool active);
    static std::string get_colored_status(const std::string &status);
    static std::string get_progress_bar(double percentage, int width = 20);
    static std::string get_badge(const std::string &text, const std::string &color);

    // Table formatting
    static void print_table_header(const std::vector<std::string> &headers,
                                   const std::vector<int> &widths);
    static void print_table_row(const std::vector<std::string> &cells,
                                const std::vector<int> &widths);
    static void print_table_separator(const std::vector<int> &widths);

    // Pause functions
    static void pause();
    static void pause_and_clear();
    static void wait_for_enter(const std::string &message = "");

    // Menu helpers with elegant design
    static void print_menu_item(int number, const std::string &text, const std::string &color = "");
    static void print_menu_separator();
    static void print_menu_category(const std::string &category);

    // Success/Error/Info messages
    static void print_success(const std::string &message);
    static void print_error(const std::string &message);
    static void print_warning(const std::string &message);
    static void print_info(const std::string &message);

    // Centered text
    static void print_centered(const std::string &text, int width = 80);

    // Clear screen
    static void clear_screen();
};

#endif // OUTPUT_FORMATTER_H
