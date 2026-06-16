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
#include "../src/latex_render.h"

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

        // ======== MAIN FUNCTIONS ======== //
        else if (event.command.get_command_name() == "tex") {
            event.thinking();
        
            std::string latex = std::get<std::string>(
                event.get_parameter("input")
            );
        
            try {
                fs::path png = Shun4miTeX::renderLaTeXSnippet(latex, fs::current_path() / "jobs");

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
        
            } catch (const std::exception& e) {
                event.edit_original_response(
                    dpp::message("Render failed, please check your LaTeX syntax.")
                );
            }
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
            if (message.rfind(tex_command, 0) == 0) {
                try {
                    std::string input = message.substr(tex_command.length() + 1);

                    fs::path png = Shun4miTeX::renderLaTeXSnippet(input, fs::current_path() / "jobs");

                    std::string owner_id = std::to_string(event.msg.author.id);

                    dpp::message msg(event.msg.channel_id,                        "Written by: <@" + owner_id + ">");

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

                } catch (const std::exception& e) {
                    bot.message_create(dpp::message(event.msg.channel_id, "Render failed, please check your LaTeX syntax."));
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

            dpp::slashcommand tex_cmd("tex", "Render LaTeX as an image", bot.me.id);tex_cmd.add_option(dpp::command_option(dpp::co_string, "input", "LaTeX input", true));
            bot.global_command_create(tex_cmd);
        }
    });

    bot.start(dpp::st_wait);

    return 0;
}