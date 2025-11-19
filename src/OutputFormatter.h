#ifndef OUTPUT_FORMATTER_H
#define OUTPUT_FORMATTER_H

#include <string>
#include <iostream>
#include <vector>

class OutputFormatter
{
public:
    // print formatted headers
    static void print_header(const std::string &title);
    static void print_sub_header(const std::string &title);
    static void print_section(const std::string &title);

    // print lines
    static void print_line(int width = 80);
    static void print_double_line(int width = 80);

    // print status indicators
    static std::string get_status_indicator(bool active);
    static std::string get_progress_bar(double percentage, int width = 20);

    // pause functions
    static void pause();
    static void pause_and_clear();

    // menu helpers
    static void print_menu_item(int number, const std::string &text, const std::string &color = "");
    static void print_menu_separator();
};

#endif // OUTPUT_FORMATTER_H
