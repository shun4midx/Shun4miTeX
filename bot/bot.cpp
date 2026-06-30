/********************************************
 * Copyright (c) 2026 Shun/修海 (@shun4midx) *
 * Project: Shun4miTeX                      *
 * File Type: C++ file                      *
 * File: bot.cpp                            *
 ****************************************** */

#include <dpp/dpp.h>
#include <dpp/user.h>
#include <dpp/cluster.h>
#include "env_parser/env_parser.h"
#include "../src/latex_render/latex_render.h"
#include "../src/natural_math/natural_math.h"
#include "../src/calculator/calculator.h"

#include <iostream>
#include <format>
#include <cstdio>
#include <fstream>
#include <filesystem>
#include <string>
#include <vector>
#include <random>
#include <ctime>
#include <cstdlib>

int main() {
    namespace fs = std::filesystem;

    std::srand(unsigned(time(NULL)));

    // Env
    static std::unordered_map<std::string, std::vector<std::string>> env = parseEnvFile();
    static std::string BOT_TOKEN = env["BOT_TOKEN"][0];
    static std::vector<std::string> MESSAGE_PERMS = env["MESSAGE_PERMS"];
    static std::string BOT_USERNAME = env["BOT_USERNAME"][0];
    static std::vector<std::string> CONFLICTING_BOTS = env["CONFLICTING_BOTS"];

    static bool correct_messages = true;

    dpp::cluster bot(BOT_TOKEN, dpp::i_default_intents | dpp::i_message_content);

    // Main Program
    bot.on_log(dpp::utility::cout_logger());

    bot.on_slashcommand([&](const dpp::slashcommand_t& event) {
        // ======== BASIC INFO ======= //
        if (event.command.get_command_name() == "shun_names") {
            event.reply("Shun\n修海\nShun/修海\nしゅう\nしゅん\nShun4mi");
        } else if (event.command.get_command_name() == "best_programming_language") {
            event.reply("C++!");
        } else if (event.command.get_command_name() == "is_cpp_good") {
            event.reply("Yes!");
        } else if (event.command.get_command_name() == "is_shun_good") {
            event.reply("Yes! *(i was forced to say that, no hes not, im held hostage by him, im scared, pls let me free)*");
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            event.edit_original_response(dpp::message("Yes! Of course! :D"));
        } else if (event.command.get_command_name() == "shun4mitex") {
            event.reply("<:Shun4miTeX:1516240280490082344>");
        }

        // ======== HELP AND SUPPORT FUNCTIONS ======== //
        else if (event.command.get_command_name() == "help_math") {
            event.reply("You can either use the slash command /math or type directly \"/math\" in your message and add a new line under it to type the math you actually want. You can type normally in English, and when you want to type math, wrap it around like so `@@<math you type>@@`. If you want to type math on a new line, wrap it instead as `@@@<math you type>@@@`. As to syntax, here are examples for more info. Feel free to copy and paste these and check out how they render!\n\n"

            "Wrap inline math:\n"
            "`@@A = pi r^2@@`\n\n"

            "Wrap display math:\n"
            "```@@@\nsum(i = 1, n, i^2)\n@@@```\n"

            "More specific examples:\n"
            "**Fractions**\n"
            "`@@frac(a+b, c)@@`\n\n"
            
            "**Roots**\n"
            "`@@sqrt(x)@@`\n"
            "`@@root(n, x)@@`\n"
            "`@@cbrt(x)@@`\n\n"
            
            "**Integrals**\n"
            "`@@int(0, 1, x^2, x)@@`\n"
            "`@@int(-infinity, infinity, e^(-x^2), x)@@`\n\n"
            
            "**Limits**\n"
            "`@@lim(x, 0, f(x))@@`\n\n"
            
            "**Summations**\n"
            "`@@sum(i = 1, n, i)@@`\n"
            "`@@prod(i = 1, n, i)@@`\n\n"
            
            "**Derivatives**\n"
            "`@@deriv(y, x)@@`\n"
            "`@@ddx(f, x)@@`\n"
            "`@@pderiv(f, x)@@`\n"
            "`@@pddx(f, x)@@`\n\n"
            
            "**Matrices**\n"
            "`@@@pmatrix(1, 2; 3, 4)@@@`\n\n"
            
            "**Sets**\n"
            "`@@RR ZZ QQ NN CC PP AA FF HH@@`\n"
            "or\n"
            "`@@ℝ ℤ ℚ ℕ ℂ ℙ 𝔸 𝔽 ℍ@@`\n\n"
            
            "**Greek**\n"
            "`@@alpha beta Gamma pi theta@@`\n"
            "or\n"
            "`@@α β Γ π θ@@`\n\n"
            
            "**Unicode also works!**\n"
            "`@@∞ ≤ ≥ ≠ → ∈ ∪ ∩ ∅ ±@@`\n\n"
            
            "**Lists (Make sure to use inside a `@@@...@@@` block!)**\n"
            "```@@@\n"
            "bullets(\n"
            "First point\n"
            "Second point\n"
            ")\n"
            "@@@```\n"
            
            "```@@@\n"
            "parts(\n"
            "Find x\n"
            "Find y\n"
            ")\n"
            "@@@```\n"
            
            "**Cases / Piecewise (Make sure to use inside a `@@@...@@@` block!)**\n"
            "```@@@\n"
            "cases(\n"
            "x^2, x >= 0\n"
            "-x, x < 0\n"
            ")\n"
            "@@@```\n"
            
            "**Binomial / Decorations**\n"
            "`@@binom(n, k)@@`\n"
            "`@@vec(v) + hat(theta) + overline(x)@@`\n\n"
            
            "For even more info, please check out https://github.com/shun4midx/Shun4miTeX");
        }

        else if (event.command.get_command_name() == "help_calc") {
            event.reply(
                "**Shun4miTeX Calculator Help**\n"
                "Use `/calc <expression>` to evaluate a scientific calculator expression.\n\n"

                "**Examples**\n"
                "`/calc 1 + 2 * 3`\n"
                "`/calc sqrt(9)`\n"
                "`/calc sin(pi / 2)`\n"
                "`/calc log(8, 2)`\n"
                "`/calc choose(5, 2)`\n"
                "`/calc deg(pi)`\n\n"

                "**Constants:** `pi`, `e`\n"
                "**Operators:** `+`, `-`, `*`, `/`, `^`, `!`\n"
                "**Grouping:** `( )`\n\n"

                "**Functions:**\n"
                "`sqrt(x)`, `root(x, n)`, `abs(x)`, `exp(x)`, `ln(x)`, `log(x)`, `log(x, b)`\n"
                "`sin(x)`, `cos(x)`, `tan(x)`, `arcsin(x)`, `arccos(x)`, `arctan(x)`\n"
                "`rad(x)`, `deg(x)`, `choose(n, k)`, `perm(n, k)`\n\n"

                "Trig functions use radians. For degrees, use `rad(x)` to first convert from degrees into radians, e.g. `/calc sin(rad(30))`.\n\n"
                "For even more info, please check out https://github.com/shun4midx/Shun4miTeX"
            );
        }

        // ======== MAIN FUNCTIONS ======== //
        else if (event.command.get_command_name() == "tex" || event.command.get_command_name() == "math") {
            event.thinking();
        
            std::string input = std::get<std::string>(event.get_parameter("input"));

            if (event.command.get_command_name() == "math") {
                input = Shun4miTeX::Natural::naturalToLaTeX(input);
                std::cout << input << std::endl;
            }
        
            try {
                fs::path png = Shun4miTeX::renderLaTeXSnippet(input, fs::current_path() / "jobs");

                std::string owner_id = std::to_string(event.command.usr.id);
        
                dpp::message msg("Written by: <@" + std::to_string(event.command.usr.id) + ">");
        
                dpp::component delete_button;
                delete_button
                    .set_type(dpp::cot_button)
                    .set_style(dpp::cos_danger)
                    .set_label("Delete")
                    .set_id("tex_delete:" + owner_id);

                dpp::component action_row;
                action_row
                    .set_type(dpp::cot_action_row)
                    .add_component(delete_button);

                msg.add_file("equation.png", dpp::utility::read_file(png.string()));
        
                msg.add_component(action_row);

                event.edit_original_response(msg);

                std::filesystem::remove_all(png.parent_path());
        
            } catch (const std::exception& e) {
                event.edit_original_response(
                    dpp::message("Render failed, please check your syntax.")
                );
            }
        }

        else if (event.command.get_command_name() == "calc") {
            std::string input = std::get<std::string>(event.get_parameter("input"));
            event.reply(Shun4miTeX::calcString(input));
        }
    });

    // ======== BUTTON LISTENERS ======== //
    bot.on_button_click([&bot](const dpp::button_click_t& event) {
        std::string custom_id = event.custom_id;
    
        const std::string prefix = "tex_delete:";
        if (custom_id.rfind(prefix, 0) != 0) {
            return;
        }
    
        std::string owner_id = custom_id.substr(prefix.size());
        std::string clicker_id = std::to_string(event.command.usr.id);
    
        if (clicker_id != owner_id) {
            event.reply(dpp::message("You can only delete your own render.").set_flags(dpp::m_ephemeral));
            return;
        }
    
        // Acknowledge the interaction
        event.reply(dpp::message("Deleted.").set_flags(dpp::m_ephemeral));
    
        // Delete the rendered message itself
        bot.message_delete(event.command.msg.id, event.command.channel_id);
    });

    // ======== LISTENING ======== //
    bot.on_message_create([&bot](const dpp::message_create_t& event) {
        // ======== MY SPECIAL FUNCTIONS ======== //
        // Carry on only if it's not from the bot itself
        if (event.msg.author.format_username() != BOT_USERNAME && !contains(CONFLICTING_BOTS, event.msg.author.format_username())) {
            std::string message = event.msg.content;

            // ======== Shun4miTeX Says (My function only) ======= //
            if (message.find("$bot_say") == 0 && contains(MESSAGE_PERMS, event.msg.author.username)) {
                std::string command = "$bot_say";
                std::string quote = message.substr(command.length() + 1, message.length() - command.length());
                bot.message_delete(event.msg.id, event.msg.channel_id);
                event.send(quote);
            }

            // ======== SPECIAL FUNCTIONS ======== //

            // ======== AUTORESPONDER ======== //
            std::string og_message = message;
            std::transform(og_message.begin(), og_message.end(), og_message.begin(), ::tolower);

            const std::string tex_command = "/tex\n";
            const std::string math_command = "/math\n";
            bool is_tex = true;
            if (message.rfind(tex_command, 0) == 0 || message.rfind(math_command, 0) == 0) {
                try {
                    std::string input = message.substr(tex_command.length());

                    if (message.rfind(math_command, 0) == 0) {
                        input = Shun4miTeX::Natural::naturalToLaTeX(input);
                        std::cout << input << std::endl;
                    }

                    fs::path png = Shun4miTeX::renderLaTeXSnippet(input, fs::current_path() / "jobs");

                    std::string owner_id = std::to_string(event.msg.author.id);

                    dpp::message msg(event.msg.channel_id, "Written by: <@" + owner_id + ">");

                    dpp::component delete_button;
                    delete_button
                        .set_type(dpp::cot_button)
                        .set_style(dpp::cos_danger)
                        .set_label("Delete")
                        .set_id("tex_delete:" + owner_id);

                    dpp::component action_row;
                    action_row
                        .set_type(dpp::cot_action_row)
                        .add_component(delete_button);

                    msg.add_file("equation.png", dpp::utility::read_file(png.string()));

                    msg.add_component(action_row);

                    bot.message_create(msg);

                    std::filesystem::remove_all(png.parent_path());

                } catch (const std::exception& e) {
                    bot.message_create(dpp::message(event.msg.channel_id, "Render failed, please check your syntax."));
                }
            }
        }
    });

    // ======== INIT PART OF THE CODE ======== //
    bot.on_ready([&bot](const dpp::ready_t& event) {
        // Bot status
        bot.set_presence(dpp::presence(dpp::ps_idle, dpp::at_game, "Helping Shun with math!"));

        if (dpp::run_once<struct register_bot_commands>()) {
            bot.global_command_create(dpp::slashcommand("shun_names", "What are all of Shun's names?", bot.me.id));
            bot.global_command_create(dpp::slashcommand("best_programming_language", "What's the best programming language?", bot.me.id));
            bot.global_command_create(dpp::slashcommand("is_cpp_good", "Is C++ good?", bot.me.id));
            bot.global_command_create(dpp::slashcommand("is_shun_good", "Is Shun good?", bot.me.id));
            bot.global_command_create(dpp::slashcommand("shun4mitex", "Sends my pfp as an emoji", bot.me.id));

            dpp::slashcommand tex_cmd("tex", "Render LaTeX as an image", bot.me.id);
            tex_cmd.add_option(dpp::command_option(dpp::co_string, "input", "LaTeX input", true));
            bot.global_command_create(tex_cmd);
            dpp::slashcommand math_cmd("math", "Render natural math as an image", bot.me.id);
            math_cmd.add_option(dpp::command_option(dpp::co_string, "input", "Natural math input", true));
            bot.global_command_create(math_cmd);

            bot.global_command_create(dpp::slashcommand("help_math", "How do I use natural math notation?", bot.me.id));
            bot.global_command_create(dpp::slashcommand("calc", "Scientific calculator mode", bot.me.id).add_option(dpp::command_option(dpp::co_string, "input", "Calculator input", true)));
            bot.global_command_create(dpp::slashcommand("help_calc", "How do I use the scientific calculator mode?", bot.me.id));
        }
    });

    bot.start(dpp::st_wait);

    return 0;
}