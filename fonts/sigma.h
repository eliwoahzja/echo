#pragma once
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#include <cstring>
#include <string>
#include <unordered_map>
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
#include "imgui.h"
#include "imgui_internal.h"
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
namespace sigma
{
    struct Context
    {
        ImFont* text_font = nullptr;
        ImFont* icon_font = nullptr;
        ImFont* tooltip_font = nullptr;
        float delta_time = 0.0f;
    };
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
    inline Context& context()
    {
        static Context value;
        return value;
    }
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
    inline ImFont* fallback_font(ImFont* font)
    {
        return font ? font : ImGui::GetFont();
    }
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
    inline void set_context(ImFont* text_font, ImFont* icon_font, ImFont* tooltip_font, float delta_time)
    {
        Context& ctx = context();
        ctx.text_font = fallback_font(text_font);
        ctx.icon_font = fallback_font(icon_font);
        ctx.tooltip_font = fallback_font(tooltip_font ? tooltip_font : text_font);
        ctx.delta_time = delta_time;
    }
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
    inline float clamp01(float value)
    {
        if (value < 0.0f)
            return 0.0f;
        if (value > 1.0f)
            return 1.0f;
        return value;
    }
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
    inline float smooth(float value)
    {
        value = clamp01(value);
        return value * value * (3.0f - 2.0f * value);
    }
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
    inline int lerp_channel(int from, int to, float fade)
    {
        return from + static_cast<int>((to - from) * fade);
    }
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
    inline bool mouse_in_rect(const ImVec2& min, const ImVec2& max)
    {
        const ImVec2 mouse = ImGui::GetIO().MousePos;
        return mouse.x >= min.x && mouse.x <= max.x && mouse.y >= min.y && mouse.y <= max.y;
    }
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
    inline bool modal_overlay(
        const char* id,
        bool& open,
        float& fade,
        const ImVec2& min,
        const ImVec2& size,
        bool allow_click_close = true)
    {
        Context& ctx = context();
        const float target = open ? 1.0f : 0.0f;
        fade += (target - fade) * (ctx.delta_time * 8.0f > 1.0f ? 1.0f : ctx.delta_time * 8.0f);
        const float alpha_fade = smooth(fade);
        if (alpha_fade <= 0.005f)
            return false;
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
        const ImVec2 max(min.x + size.x, min.y + size.y);
        const auto draw_modal_fade = [&](ImDrawList* draw_list)
        {
            const int alpha = static_cast<int>(164.0f * alpha_fade);
            draw_list->AddRectFilled(min, max, IM_COL32(0, 0, 0, alpha), 4.0f);
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
            const int soft_alpha = static_cast<int>(54.0f * alpha_fade);
            draw_list->AddRectFilled(
                ImVec2(min.x + 1.0f, min.y + 1.0f),
                ImVec2(max.x - 1.0f, max.y - 1.0f),
                IM_COL32(12, 12, 12, soft_alpha),
                4.0f);
        };
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
        if (!allow_click_close)
        {
            draw_modal_fade(ImGui::GetForegroundDrawList());
            if (ImGui::IsKeyPressed(ImGuiKey_Escape))
                open = false;
            return false;
        }
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
        ImGui::SetNextWindowPos(min);
        ImGui::SetNextWindowSize(size);
        ImGuiWindowFlags flags =
            ImGuiWindowFlags_NoDecoration |
            ImGuiWindowFlags_NoMove |
            ImGuiWindowFlags_NoSavedSettings |
            ImGuiWindowFlags_NoBackground |
            ImGuiWindowFlags_NoScrollbar |
            ImGuiWindowFlags_NoScrollWithMouse |
            ImGuiWindowFlags_NoBringToFrontOnFocus;
        ImGui::Begin(id, nullptr, flags);
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
        ImDrawList* draw_list = ImGui::GetWindowDrawList();
        draw_modal_fade(draw_list);
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
        bool clicked = false;
        if (allow_click_close)
        {
            ImGui::SetCursorPos(ImVec2(0.0f, 0.0f));
            ImGui::InvisibleButton("##overlay_hit", size);
            clicked = ImGui::IsItemClicked(ImGuiMouseButton_Left);
        }
        ImGui::End();
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
        if ((allow_click_close && clicked) || ImGui::IsKeyPressed(ImGuiKey_Escape))
            open = false;
        return clicked;
    }
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
    inline void modal_title(
        const ImVec2& min,
        const char* icon,
        const char* label,
        float alpha,
        float icon_size = 16.0f,
        float text_size = 16.0f)
    {
        Context& ctx = context();
        const float draw_alpha = smooth(alpha);
        if (draw_alpha <= 0.005f)
            return;
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
        ImFont* icon_font = fallback_font(ctx.icon_font);
        ImFont* text_font = fallback_font(ctx.text_font);
        const int a = static_cast<int>(255.0f * draw_alpha);
        const ImVec2 icon_measure = icon_font->CalcTextSizeA(icon_size, 1000.0f, 0.0f, icon);
        const ImVec2 text_measure = text_font->CalcTextSizeA(text_size, 1000.0f, 0.0f, label);
        const float center_y = min.y + text_measure.y * 0.5f;
        ImDrawList* draw_list = ImGui::GetForegroundDrawList();
        draw_list->AddText(
            icon_font,
            icon_size,
            ImVec2(min.x, center_y - icon_measure.y * 0.5f),
            IM_COL32(142, 134, 246, a),
            icon);
        draw_list->AddText(
            text_font,
            text_size,
            ImVec2(min.x + icon_measure.x + 17.0f, min.y),
            IM_COL32(235, 235, 235, a),
            label);
    }
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
    inline void modal_title_centered(
        const ImVec2& center,
        const char* icon,
        const char* label,
        float alpha,
        float icon_size = 20.0f,
        float text_size = 18.0f)
    {
        Context& ctx = context();
        const float draw_alpha = smooth(alpha);
        if (draw_alpha <= 0.005f)
            return;
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
        ImFont* icon_font = fallback_font(ctx.icon_font);
        ImFont* text_font = fallback_font(ctx.text_font);
        const ImVec2 icon_measure = icon_font->CalcTextSizeA(icon_size, 1000.0f, 0.0f, icon);
        const ImVec2 text_measure = text_font->CalcTextSizeA(text_size, 1000.0f, 0.0f, label);
        const float gap = 18.0f;
        const float content_width = icon_measure.x + gap + text_measure.x;
        const float start_x = center.x - content_width * 0.5f;
        const int a = static_cast<int>(255.0f * draw_alpha);
        ImDrawList* draw_list = ImGui::GetForegroundDrawList();
        draw_list->AddText(
            icon_font,
            icon_size,
            ImVec2(start_x, center.y - icon_measure.y * 0.5f),
            IM_COL32(142, 134, 246, a),
            icon);
        draw_list->AddText(
            text_font,
            text_size,
            ImVec2(start_x + icon_measure.x + gap, center.y - text_measure.y * 0.5f),
            IM_COL32(242, 242, 242, a),
            label);
    }
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
    inline bool modal_close_button(
        const char* id,
        const ImVec2& center,
        float alpha,
        float& hover_fade,
        const char* icon = u8"\uf44f",
        float icon_size = 16.0f)
    {
        Context& ctx = context();
        const float draw_alpha = smooth(alpha);
        if (draw_alpha <= 0.005f)
            return false;
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
        const ImVec2 hit_size(28.0f, 28.0f);
        const ImVec2 min(center.x - hit_size.x * 0.5f, center.y - hit_size.y * 0.5f);
        const ImVec2 max(min.x + hit_size.x, min.y + hit_size.y);
        const bool hovered = mouse_in_rect(min, max);
        const bool clicked = hovered && ImGui::IsMouseClicked(ImGuiMouseButton_Left);
        if (hovered)
            ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
        hover_fade += ((hovered ? 1.0f : 0.0f) - hover_fade) * (ctx.delta_time * 12.0f > 1.0f ? 1.0f : ctx.delta_time * 12.0f);
        const float hover = smooth(hover_fade);
        ImFont* icon_font = fallback_font(ctx.icon_font);
        const ImVec2 icon_measure = icon_font->CalcTextSizeA(icon_size, 1000.0f, 0.0f, icon);
        const int a = static_cast<int>(255.0f * draw_alpha);
        ImGui::GetForegroundDrawList()->AddText(
            icon_font,
            icon_size,
            ImVec2(center.x - icon_measure.x * 0.5f, center.y - icon_measure.y * 0.5f),
            IM_COL32(
                lerp_channel(194, 235, hover),
                lerp_channel(72, 92, hover),
                lerp_channel(76, 96, hover),
                a),
            icon);
        return clicked;
    }
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
    inline bool modal_button(
        const char* label,
        const char* icon,
        const ImVec2& min,
        const ImVec2& size,
        float alpha,
        float& hover_fade,
        float text_size = 12.0f,
        float icon_size = 13.0f)
    {
        Context& ctx = context();
        const float draw_alpha = smooth(alpha);
        if (draw_alpha <= 0.005f)
            return false;
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
        const ImVec2 max(min.x + size.x, min.y + size.y);
        const bool hovered = mouse_in_rect(min, max);
        if (hovered)
            ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
        const bool clicked = hovered && ImGui::IsMouseClicked(ImGuiMouseButton_Left);
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
        hover_fade += ((hovered ? 1.0f : 0.0f) - hover_fade) * (ctx.delta_time * 8.0f > 1.0f ? 1.0f : ctx.delta_time * 8.0f);
        const float hover = smooth(hover_fade);
        const int r = lerp_channel(148, 129, hover);
        const int g = lerp_channel(140, 122, hover);
        const int b = lerp_channel(246, 222, hover);
        const int a = static_cast<int>(255.0f * draw_alpha);
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
        ImDrawList* draw_list = ImGui::GetForegroundDrawList();
        draw_list->AddRectFilled(min, max, IM_COL32(r, g, b, a), 2.0f);
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
        ImFont* text_font = fallback_font(ctx.text_font);
        ImFont* icon_font = fallback_font(ctx.icon_font);
        const ImVec2 icon_measure = icon_font->CalcTextSizeA(icon_size, 1000.0f, 0.0f, icon);
        const ImVec2 text_measure = text_font->CalcTextSizeA(text_size, 1000.0f, 0.0f, label);
        const float gap = 12.0f;
        const float content_width = icon_measure.x + gap + text_measure.x;
        const float start_x = min.x + (size.x - content_width) * 0.5f;
        const float center_y = min.y + size.y * 0.5f;
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
        draw_list->AddText(
            icon_font,
            icon_size,
            ImVec2(start_x, center_y - icon_measure.y * 0.5f),
            IM_COL32(0, 0, 0, a),
            icon);
        draw_list->AddText(
            text_font,
            text_size,
            ImVec2(start_x + icon_measure.x + gap, center_y - text_measure.y * 0.5f),
            IM_COL32(0, 0, 0, a),
            label);
        return clicked;
    }
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
    struct ModalInputState
    {
        int cursor_pos = 0;
        int selection_start = 0;
        int selection_end = 0;
        int selection_anchor = 0;
        float scroll_x = 0.0f;
        bool selection_dragging = false;
    };
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
    inline bool should_show_caret(ImGuiID id, bool active, bool changed, int cursor_pos)
    {
        static std::unordered_map<ImGuiID, double> last_activity_times;
        static std::unordered_map<ImGuiID, int> last_cursor_positions;
        static std::unordered_map<ImGuiID, bool> last_active_states;
        double current_time = ImGui::GetTime();
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
        const bool was_active = last_active_states[id];
        last_active_states[id] = active;
        if (!active)
            return false;
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
        auto cursor_it = last_cursor_positions.find(id);
        auto activity_it = last_activity_times.find(id);
        const bool activated = !was_active;
        if (activated || changed || cursor_it == last_cursor_positions.end() || activity_it == last_activity_times.end() || cursor_pos != cursor_it->second)
        {
            last_activity_times[id] = current_time;
            last_cursor_positions[id] = cursor_pos;
        }
        double time_since_activity = current_time - last_activity_times[id];
        if (time_since_activity < 0.55)
            return true;
        return (static_cast<int>((time_since_activity - 0.55) * 2.0) % 2 == 0);
    }
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
    inline int text_length_bytes(const char* text)
    {
        int length = 0;
        while (text && text[length] != '\0')
            ++length;
        return length;
    }
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
    inline int clamp_text_pos(int value, int length)
    {
        if (value < 0)
            return 0;
        if (value > length)
            return length;
        return value;
    }
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
    inline float text_width_to(ImFont* font, float size, const char* text, int byte_count)
    {
        if (!text || byte_count <= 0)
            return 0.0f;
        return font->CalcTextSizeA(size, 10000.0f, 0.0f, text, text + byte_count).x;
    }
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
    inline int modal_input_state_callback(ImGuiInputTextCallbackData* data)
    {
        ModalInputState* state = static_cast<ModalInputState*>(data->UserData);
        if (!state)
            return 0;
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
        const int length = data->BufTextLen;
        state->cursor_pos = clamp_text_pos(data->CursorPos, length);
        state->selection_start = clamp_text_pos(data->SelectionStart, length);
        state->selection_end = clamp_text_pos(data->SelectionEnd, length);
        return 0;
    }
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
    struct CodeEditorState
    {
        int cursor_pos = 0;
        int selection_start = 0;
        int selection_end = 0;
        int selection_anchor = 0;
        float scroll_x = 0.0f;
        float scroll_y = 0.0f;
        bool dragging_x = false;
        bool dragging_y = false;
        bool selection_dragging = false;
    };
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
    inline ImGuiID& active_manual_text_input_id()
    {
        static ImGuiID value = 0;
        return value;
    }
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
    inline bool manual_text_input_active(ImGuiID id)
    {
        return id != 0 && active_manual_text_input_id() == id;
    }
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
    inline int previous_text_pos(const char* value, int cursor)
    {
        if (cursor <= 0)
            return 0;
        int pos = cursor - 1;
        while (pos > 0 && (static_cast<unsigned char>(value[pos]) & 0xC0) == 0x80)
            --pos;
        return pos;
    }
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
    inline int next_text_pos(const char* value, int cursor)
    {
        const int length = text_length_bytes(value);
        if (cursor >= length)
            return length;
        int pos = cursor + 1;
        while (pos < length && (static_cast<unsigned char>(value[pos]) & 0xC0) == 0x80)
            ++pos;
        return pos;
    }
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
    inline bool is_word_byte(unsigned char value)
    {
        return (value >= 'a' && value <= 'z') ||
               (value >= 'A' && value <= 'Z') ||
               (value >= '0' && value <= '9') ||
               value == '_';
    }
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
    inline bool is_space_byte(unsigned char value)
    {
        return value == ' ' || value == '\t' || value == '\r' || value == '\n';
    }
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
    inline bool is_word_separator(const char* value, int pos)
    {
        const unsigned char byte = static_cast<unsigned char>(value[pos]);
        if ((byte & 0x80) != 0)
            return false;
        return !is_word_byte(byte);
    }
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
    inline int previous_word_text_pos(const char* value, int cursor)
    {
        const int length = text_length_bytes(value);
        cursor = clamp_text_pos(cursor, length);
        if (cursor <= 0)
            return 0;
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
        int pos = previous_text_pos(value, cursor);
        while (pos > 0 && is_space_byte(static_cast<unsigned char>(value[pos])))
            pos = previous_text_pos(value, pos);
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
        const bool separator = is_word_separator(value, pos);
        while (pos > 0)
        {
            const int prev = previous_text_pos(value, pos);
            if (is_word_separator(value, prev) != separator || (separator && is_space_byte(static_cast<unsigned char>(value[prev]))))
                break;
            pos = prev;
        }
        return pos;
    }
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
    inline int next_word_text_pos(const char* value, int cursor)
    {
        const int length = text_length_bytes(value);
        cursor = clamp_text_pos(cursor, length);
        if (cursor >= length)
            return length;
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
        int pos = cursor;
        while (pos < length && is_space_byte(static_cast<unsigned char>(value[pos])))
            pos = next_text_pos(value, pos);
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
        if (pos >= length)
            return length;
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
        const bool separator = is_word_separator(value, pos);
        while (pos < length)
        {
            const int next = next_text_pos(value, pos);
            if (next >= length)
                return length;
            if (is_word_separator(value, next) != separator || (separator && is_space_byte(static_cast<unsigned char>(value[next]))))
                return next;
            pos = next;
        }
        return length;
    }
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
    inline int text_pos_from_mouse_x(const char* value, ImFont* font, float size, float mouse_x, float text_x, float scroll_x)
    {
        const int length = text_length_bytes(value);
        if (!value || length <= 0)
            return 0;
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
        int previous = 0;
        for (int pos = next_text_pos(value, 0); pos <= length; pos = next_text_pos(value, pos))
        {
            const float prev_x = text_x + text_width_to(font, size, value, previous) - scroll_x;
            const float curr_x = text_x + text_width_to(font, size, value, pos) - scroll_x;
            const float midpoint = (prev_x + curr_x) * 0.5f;
            if (mouse_x < midpoint)
                return previous;
            if (pos == length)
                break;
            previous = pos;
        }
        return length;
    }
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
    inline bool erase_text_range(char* value, size_t value_size, int& cursor, int start, int end)
    {
        (void)value_size;
        const int length = text_length_bytes(value);
        start = clamp_text_pos(start, length);
        end = clamp_text_pos(end, length);
        if (start > end)
        {
            const int tmp = start;
            start = end;
            end = tmp;
        }
        if (start == end)
            return false;
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
        std::memmove(value + start, value + end, static_cast<size_t>(length - end + 1));
        cursor = start;
        return true;
    }
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
    inline bool insert_text_at_cursor(char* value, size_t value_size, int& cursor, const char* text)
    {
        if (!text || text[0] == '\0' || value_size == 0)
            return false;
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
        const int length = text_length_bytes(value);
        cursor = clamp_text_pos(cursor, length);
        const int insert_length = text_length_bytes(text);
        const int capacity = static_cast<int>(value_size) - 1;
        if (capacity <= length)
            return false;
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
        const int writable_length = insert_length < capacity - length ? insert_length : capacity - length;
        if (writable_length <= 0)
            return false;
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
        std::memmove(value + cursor + writable_length, value + cursor, static_cast<size_t>(length - cursor + 1));
        std::memcpy(value + cursor, text, static_cast<size_t>(writable_length));
        cursor += writable_length;
        return true;
    }
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
    inline bool read_clipboard_utf8(char* out, size_t out_size)
    {
        if (!out || out_size == 0)
            return false;
        out[0] = '\0';
        if (!OpenClipboard(nullptr))
            return false;
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
        bool ok = false;
        HANDLE handle = GetClipboardData(CF_UNICODETEXT);
        if (handle)
        {
            const wchar_t* wide = static_cast<const wchar_t*>(GlobalLock(handle));
            if (wide)
            {
                const int written = WideCharToMultiByte(CP_UTF8, 0, wide, -1, out, static_cast<int>(out_size), nullptr, nullptr);
                ok = written > 0;
                GlobalUnlock(handle);
            }
        }
        CloseClipboard();
        return ok;
    }
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
    inline bool write_clipboard_utf8(const char* text, int text_length)
    {
        if (!text || text_length <= 0)
            return false;
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
        const int wide_length = MultiByteToWideChar(CP_UTF8, 0, text, text_length, nullptr, 0);
        if (wide_length <= 0)
            return false;
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
        HGLOBAL handle = GlobalAlloc(GMEM_MOVEABLE, static_cast<SIZE_T>(wide_length + 1) * sizeof(wchar_t));
        if (!handle)
            return false;
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
        wchar_t* wide = static_cast<wchar_t*>(GlobalLock(handle));
        if (!wide)
        {
            GlobalFree(handle);
            return false;
        }
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
        MultiByteToWideChar(CP_UTF8, 0, text, text_length, wide, wide_length);
        wide[wide_length] = L'\0';
        GlobalUnlock(handle);
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
        if (!OpenClipboard(nullptr))
        {
            GlobalFree(handle);
            return false;
        }
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
        EmptyClipboard();
        const bool ok = SetClipboardData(CF_UNICODETEXT, handle) != nullptr;
        CloseClipboard();
        if (!ok)
            GlobalFree(handle);
        return ok;
    }
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
    inline bool manual_text_key_pressed(int vk, bool repeat)
    {
        struct KeyState
        {
            bool down[256] = {};
            double next_repeat[256] = {};
        };
        static KeyState state;
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
        const bool is_down = (GetAsyncKeyState(vk) & 0x8000) != 0;
        const double now = ImGui::GetTime();
        bool pressed = false;
        if (is_down)
        {
            if (!state.down[vk])
            {
                pressed = true;
                state.next_repeat[vk] = now + 0.38;
            }
            else if (repeat && now >= state.next_repeat[vk])
            {
                pressed = true;
                state.next_repeat[vk] = now + 0.045;
            }
        }
        state.down[vk] = is_down;
        return pressed;
    }
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
    inline bool manual_text_vk_is_text(int vk)
    {
        if (vk >= VK_F1 && vk <= VK_F24)
            return false;
        switch (vk)
        {
        case VK_LBUTTON:
        case VK_RBUTTON:
        case VK_CANCEL:
        case VK_MBUTTON:
        case VK_XBUTTON1:
        case VK_XBUTTON2:
        case VK_BACK:
        case VK_TAB:
        case VK_RETURN:
        case VK_SHIFT:
        case VK_CONTROL:
        case VK_MENU:
        case VK_PAUSE:
        case VK_CAPITAL:
        case VK_ESCAPE:
        case VK_SPACE:
        case VK_PRIOR:
        case VK_NEXT:
        case VK_END:
        case VK_HOME:
        case VK_LEFT:
        case VK_UP:
        case VK_RIGHT:
        case VK_DOWN:
        case VK_SELECT:
        case VK_PRINT:
        case VK_EXECUTE:
        case VK_SNAPSHOT:
        case VK_INSERT:
        case VK_DELETE:
        case VK_HELP:
        case VK_LWIN:
        case VK_RWIN:
        case VK_APPS:
        case VK_NUMLOCK:
        case VK_SCROLL:
        case VK_LSHIFT:
        case VK_RSHIFT:
        case VK_LCONTROL:
        case VK_RCONTROL:
        case VK_LMENU:
        case VK_RMENU:
            return false;
        default:
            return vk >= 0x30;
        }
    }
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
    template <typename StateT>
    inline bool manual_text_input(
        ImGuiID id,
        const ImVec2& min,
        const ImVec2& max,
        char* value,
        size_t value_size,
        StateT& state,
        bool multiline)
    {
        if (id == 0 || !value || value_size == 0)
            return false;
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
        const bool hovered = mouse_in_rect(min, max);
        ImGuiID& active_id = active_manual_text_input_id();
        if (ImGui::IsMouseClicked(ImGuiMouseButton_Left))
        {
            if (hovered)
            {
                active_id = id;
                const int length = text_length_bytes(value);
                state.cursor_pos = clamp_text_pos(state.cursor_pos, length);
                state.selection_anchor = state.cursor_pos;
                state.selection_start = state.selection_anchor;
                state.selection_end = state.cursor_pos;
                state.selection_dragging = true;
            }
            else if (active_id == id)
            {
                active_id = 0;
                state.selection_dragging = false;
            }
        }
        if (active_id == id && state.selection_dragging && !ImGui::IsMouseDown(ImGuiMouseButton_Left))
            state.selection_dragging = false;
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
        if (active_id != id)
            return false;
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
        ImGui::SetNextFrameWantCaptureKeyboard(true);
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
        bool changed = false;
        int cursor = clamp_text_pos(state.cursor_pos, text_length_bytes(value));
        int selection_start = clamp_text_pos(state.selection_start, text_length_bytes(value));
        int selection_end = clamp_text_pos(state.selection_end, text_length_bytes(value));
        if (selection_start > selection_end)
        {
            const int tmp = selection_start;
            selection_start = selection_end;
            selection_end = tmp;
        }
        state.selection_start = selection_start;
        state.selection_end = selection_end;
        auto clear_selection = [&]()
        {
            state.selection_start = cursor;
            state.selection_end = cursor;
            state.selection_anchor = cursor;
        };
        auto copy_selection = [&]() -> bool
        {
            const int length = text_length_bytes(value);
            int start = clamp_text_pos(state.selection_start, length);
            int end = clamp_text_pos(state.selection_end, length);
            if (start > end)
            {
                const int tmp = start;
                start = end;
                end = tmp;
            }
            if (start == end)
                return false;
            return write_clipboard_utf8(value + start, end - start);
        };
        auto erase_selection = [&]() -> bool
        {
            if (state.selection_start == state.selection_end)
                return false;
            bool erased = erase_text_range(value, value_size, cursor, state.selection_start, state.selection_end);
            state.cursor_pos = cursor;
            clear_selection();
            return erased;
        };
        auto insert_text = [&](const char* text) -> bool
        {
            erase_selection();
            bool inserted = insert_text_at_cursor(value, value_size, cursor, text);
            state.cursor_pos = cursor;
            clear_selection();
            return inserted;
        };
        auto move_cursor = [&](int new_cursor, bool selecting)
        {
            const int length = text_length_bytes(value);
            const int old_cursor = cursor;
            cursor = clamp_text_pos(new_cursor, length);
            state.cursor_pos = cursor;
            if (selecting)
            {
                if (state.selection_start == state.selection_end)
                    state.selection_anchor = old_cursor;
                state.selection_start = state.selection_anchor < cursor ? state.selection_anchor : cursor;
                state.selection_end = state.selection_anchor > cursor ? state.selection_anchor : cursor;
            }
            else
            {
                clear_selection();
            }
        };
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
        const bool ctrl_down = (GetAsyncKeyState(VK_CONTROL) & 0x8000) != 0 ||
                               (GetAsyncKeyState(VK_LCONTROL) & 0x8000) != 0 ||
                               (GetAsyncKeyState(VK_RCONTROL) & 0x8000) != 0;
        const bool shift_down = (GetAsyncKeyState(VK_SHIFT) & 0x8000) != 0 ||
                                (GetAsyncKeyState(VK_LSHIFT) & 0x8000) != 0 ||
                                (GetAsyncKeyState(VK_RSHIFT) & 0x8000) != 0;
        const bool alt_down = (GetAsyncKeyState(VK_MENU) & 0x8000) != 0 ||
                              (GetAsyncKeyState(VK_LMENU) & 0x8000) != 0 ||
                              (GetAsyncKeyState(VK_RMENU) & 0x8000) != 0;
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
        if (ctrl_down && manual_text_key_pressed('A', false))
        {
            state.selection_start = 0;
            state.selection_end = text_length_bytes(value);
            state.cursor_pos = state.selection_end;
            state.selection_anchor = 0;
        }
        if (ctrl_down && manual_text_key_pressed('C', false))
            copy_selection();
        if (ctrl_down && manual_text_key_pressed('X', false))
        {
            if (copy_selection() && erase_selection())
                changed = true;
        }
        if (ctrl_down && manual_text_key_pressed('V', false))
        {
            char clipboard[2048];
            if (read_clipboard_utf8(clipboard, sizeof(clipboard)))
                changed |= insert_text(clipboard);
        }
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
        if (manual_text_key_pressed(VK_BACK, true))
        {
            if (erase_selection())
            {
                changed = true;
            }
            else if (cursor > 0)
            {
                const int prev = ctrl_down ? previous_word_text_pos(value, cursor) : previous_text_pos(value, cursor);
                changed |= erase_text_range(value, value_size, cursor, prev, cursor);
                state.cursor_pos = cursor;
                clear_selection();
            }
        }
        if (manual_text_key_pressed(VK_DELETE, true))
        {
            if (erase_selection())
            {
                changed = true;
            }
            else
            {
                const int length = text_length_bytes(value);
                if (cursor < length)
                {
                    const int next = ctrl_down ? next_word_text_pos(value, cursor) : next_text_pos(value, cursor);
                    changed |= erase_text_range(value, value_size, cursor, cursor, next);
                    state.cursor_pos = cursor;
                    clear_selection();
                }
            }
        }
        if (manual_text_key_pressed(VK_LEFT, true))
        {
            move_cursor(ctrl_down ? previous_word_text_pos(value, cursor) : previous_text_pos(value, cursor), shift_down);
        }
        if (manual_text_key_pressed(VK_RIGHT, true))
        {
            move_cursor(ctrl_down ? next_word_text_pos(value, cursor) : next_text_pos(value, cursor), shift_down);
        }
        if (manual_text_key_pressed(VK_HOME, true))
        {
            move_cursor(0, shift_down);
        }
        if (manual_text_key_pressed(VK_END, true))
        {
            move_cursor(text_length_bytes(value), shift_down);
        }
        if (manual_text_key_pressed(VK_ESCAPE, false))
        {
            active_id = 0;
        }
        if (manual_text_key_pressed(VK_RETURN, true))
        {
            if (multiline)
                changed |= insert_text("\n");
            else
                active_id = 0;
        }
        if (multiline && manual_text_key_pressed(VK_TAB, true))
            changed |= insert_text("    ");
        if (manual_text_key_pressed(VK_SPACE, true) && !ctrl_down && !alt_down)
            changed |= insert_text(" ");
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
        if (!ctrl_down && !alt_down)
        {
            BYTE keyboard_state[256] = {};
            for (int key = 0; key < 256; ++key)
                keyboard_state[key] = (GetAsyncKeyState(key) & 0x8000) ? 0x80 : 0x00;
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
            for (int vk = 0x20; vk < 256; ++vk)
            {
                if (!manual_text_vk_is_text(vk))
                    continue;
                if (!manual_text_key_pressed(vk, true))
                    continue;
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                wchar_t wide_chars[8] = {};
                const UINT scan_code = MapVirtualKeyW(static_cast<UINT>(vk), MAPVK_VK_TO_VSC);
                const int wide_count = ToUnicode(static_cast<UINT>(vk), scan_code, keyboard_state, wide_chars, IM_ARRAYSIZE(wide_chars), 0);
                if (wide_count <= 0)
                    continue;
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
                char utf8[32] = {};
                const int byte_count = WideCharToMultiByte(CP_UTF8, 0, wide_chars, wide_count, utf8, IM_ARRAYSIZE(utf8) - 1, nullptr, nullptr);
                if (byte_count > 0)
                {
                    utf8[byte_count] = '\0';
                    changed |= insert_text(utf8);
                }
            }
        }
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
        const int length = text_length_bytes(value);
        state.cursor_pos = clamp_text_pos(state.cursor_pos, length);
        state.selection_start = clamp_text_pos(state.selection_start, length);
        state.selection_end = clamp_text_pos(state.selection_end, length);
        return changed;
    }
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
    inline int code_editor_state_callback(ImGuiInputTextCallbackData* data)
    {
        CodeEditorState* state = static_cast<CodeEditorState*>(data->UserData);
        if (!state)
            return 0;
        state->cursor_pos = clamp_text_pos(data->CursorPos, data->BufTextLen);
        state->selection_start = clamp_text_pos(data->SelectionStart, data->BufTextLen);
        state->selection_end = clamp_text_pos(data->SelectionEnd, data->BufTextLen);
        return 0;
    }
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
    inline bool text_starts_with(const char* text, const char* token)
    {
        int index = 0;
        while (token[index] != '\0')
        {
            if (text[index] != token[index])
                return false;
            ++index;
        }
        return true;
    }
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
    inline bool is_word_char(char value)
    {
        return (value >= 'a' && value <= 'z') || (value >= 'A' && value <= 'Z') || (value >= '0' && value <= '9') || value == '_';
    }
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
    inline bool token_equals(const char* text, int length, const char* token)
    {
        int index = 0;
        while (index < length && token[index] != '\0')
        {
            if (text[index] != token[index])
                return false;
            ++index;
        }
        return index == length && token[index] == '\0';
    }
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
    inline bool token_in_list(const char* text, int length, const char* const* tokens, int token_count)
    {
        for (int index = 0; index < token_count; ++index)
        {
            if (token_equals(text, length, tokens[index]))
                return true;
        }
        return false;
    }
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
    inline int code_editor_line_count(const char* text)
    {
        int count = 1;
        for (int index = 0; text[index] != '\0'; ++index)
        {
            if (text[index] == '\n')
                ++count;
        }
        return count;
    }
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
    inline void code_editor_line_range(const char* text, int line_index, int* start, int* end)
    {
        int current_line = 0;
        int line_start = 0;
        for (int index = 0; ; ++index)
        {
            if (text[index] == '\n' || text[index] == '\0')
            {
                if (current_line == line_index)
                {
                    *start = line_start;
                    *end = index;
                    return;
                }
                if (text[index] == '\0')
                    break;
                ++current_line;
                line_start = index + 1;
            }
        }
        *start = line_start;
        *end = line_start;
    }
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
    inline int code_editor_char_from_mouse(
        const char* text,
        ImFont* font,
        float font_size,
        const ImVec2& mouse_pos,
        float text_left,
        float content_top,
        float line_height,
        float scroll_x,
        float scroll_y)
    {
        const int line_count = code_editor_line_count(text);
        int line_index = static_cast<int>((mouse_pos.y - content_top + scroll_y) / line_height);
        if (line_index < 0)
            line_index = 0;
        if (line_index >= line_count)
            line_index = line_count - 1;
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
        int line_start = 0;
        int line_end = 0;
        code_editor_line_range(text, line_index, &line_start, &line_end);
        const float local_x = mouse_pos.x - text_left + scroll_x;
        if (local_x <= 0.0f)
            return line_start;
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
        float previous_width = 0.0f;
        for (int index = line_start; index < line_end; ++index)
        {
            const float next_width = text_width_to(font, font_size, text + line_start, index + 1 - line_start);
            if (local_x < (previous_width + next_width) * 0.5f)
                return index;
            previous_width = next_width;
        }
        return line_end;
    }
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
    inline void code_editor_cursor_line(const char* text, int cursor, int* line_index, int* line_start, int* line_end)
    {
        const int length = text_length_bytes(text);
        cursor = clamp_text_pos(cursor, length);
        int current_line = 0;
        int start = 0;
        for (int index = 0; ; ++index)
        {
            if (text[index] == '\n' || text[index] == '\0')
            {
                if (cursor <= index || text[index] == '\0')
                {
                    *line_index = current_line;
                    *line_start = start;
                    *line_end = index;
                    return;
                }
                ++current_line;
                start = index + 1;
            }
        }
    }
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
    inline int code_editor_char_from_line_x(const char* text, ImFont* font, float font_size, int line_index, float target_x)
    {
        const int line_count = code_editor_line_count(text);
        if (line_index < 0)
            line_index = 0;
        if (line_index >= line_count)
            line_index = line_count - 1;
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
        int line_start = 0;
        int line_end = 0;
        code_editor_line_range(text, line_index, &line_start, &line_end);
        if (target_x <= 0.0f)
            return line_start;
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
        float previous_width = 0.0f;
        for (int index = line_start; index < line_end; ++index)
        {
            const float next_width = text_width_to(font, font_size, text + line_start, index + 1 - line_start);
            if (target_x < (previous_width + next_width) * 0.5f)
                return index;
            previous_width = next_width;
        }
        return line_end;
    }
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
    inline bool code_editor_word_at(const char* text, int position, int* selection_start, int* selection_end)
    {
        const int length = text_length_bytes(text);
        int cursor = clamp_text_pos(position, length);
        if (cursor >= length && cursor > 0)
            --cursor;
        if (cursor > 0 && (cursor >= length || !is_word_char(text[cursor])) && is_word_char(text[cursor - 1]))
            --cursor;
        if (cursor < 0 || cursor >= length || !is_word_char(text[cursor]))
            return false;
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
        int start = cursor;
        int end = cursor + 1;
        while (start > 0 && is_word_char(text[start - 1]))
            --start;
        while (end < length && is_word_char(text[end]))
            ++end;
        *selection_start = start;
        *selection_end = end;
        return start < end;
    }
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
    inline void format_positive_int(char* output, int output_size, int value)
    {
        if (output_size <= 0)
            return;
        if (value < 0)
            value = 0;
        char reversed[16];
        int count = 0;
        do
        {
            reversed[count++] = static_cast<char>('0' + (value % 10));
            value /= 10;
        } while (value > 0 && count < IM_ARRAYSIZE(reversed));
        int write_count = count < output_size - 1 ? count : output_size - 1;
        for (int index = 0; index < write_count; ++index)
            output[index] = reversed[count - 1 - index];
        output[write_count] = '\0';
    }
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
    inline void draw_lua_syntax_line(
        ImDrawList* draw_list,
        ImFont* font,
        float font_size,
        const char* line,
        int length,
        const ImVec2& pos,
        float scroll_x,
        int alpha)
    {
        float x = pos.x - scroll_x;
        int index = 0;
        while (index < length)
        {
            ImU32 color = IM_COL32(210, 210, 214, alpha);
            int token_end = index + 1;
            if (index + 1 < length && line[index] == '-' && line[index + 1] == '-')
            {
                color = IM_COL32(109, 151, 106, alpha);
                token_end = length;
            }
            else if (line[index] == '"' || line[index] == '\'')
            {
                const char quote = line[index];
                color = IM_COL32(221, 198, 122, alpha);
                token_end = index + 1;
                while (token_end < length)
                {
                    if (line[token_end] == quote)
                    {
                        ++token_end;
                        break;
                    }
                    ++token_end;
                }
            }
            else if (is_word_char(line[index]))
            {
                token_end = index + 1;
                while (token_end < length && is_word_char(line[token_end]))
                    ++token_end;
                const int token_length = token_end - index;
                static const char* const lua_keywords[] = {
                    "and", "break", "do", "else", "elseif", "end", "false", "for", "function", "if",
                    "in", "local", "nil", "not", "or", "repeat", "return", "then", "true", "until", "while"
                };
                static const char* const lua_globals[] = {
                    "assert", "collectgarbage", "dofile", "error", "getmetatable", "ipairs", "load", "next",
                    "pairs", "pcall", "print", "rawequal", "rawget", "rawlen", "rawset", "require",
                    "select", "setmetatable", "tonumber", "tostring", "type", "xpcall",
                    "string", "table", "math", "coroutine", "os", "debug", "utf8"
                };
                static const char* const fivem_functions[] = {
                    "TriggerServerEvent", "TriggerEvent", "RegisterNetEvent", "RegisterServerEvent",
                    "AddEventHandler", "RemoveEventHandler", "CancelEvent", "WasEventCanceled",
                    "CreateThread", "Citizen", "Wait", "SetTimeout", "GetGameTimer",
                    "PlayerPedId", "PlayerId", "GetPlayerPed", "GetPlayerServerId", "GetPlayerName",
                    "GetEntityCoords", "SetEntityCoords", "GetEntityHeading", "SetEntityHeading",
                    "GetVehiclePedIsIn", "GetPedInVehicleSeat", "IsPedInAnyVehicle",
                    "GetHashKey", "RequestModel", "HasModelLoaded", "SetModelAsNoLongerNeeded",
                    "NetworkGetNetworkIdFromEntity", "NetworkGetEntityFromNetworkId",
                    "SendNUIMessage", "SetNuiFocus", "RegisterNUICallback",
                    "exports", "GetResourceState", "GetCurrentResourceName",
                    "ExecuteCommand", "LoadResourceFile", "SaveResourceFile"
                };
                if (token_equals(line + index, token_length, "print") ||
                    token_in_list(line + index, token_length, lua_keywords, IM_ARRAYSIZE(lua_keywords)))
                {
                    color = IM_COL32(112, 190, 178, alpha);
                }
                else if (token_in_list(line + index, token_length, fivem_functions, IM_ARRAYSIZE(fivem_functions)))
                {
                    color = IM_COL32(148, 141, 246, alpha);
                }
                else if (token_in_list(line + index, token_length, lua_globals, IM_ARRAYSIZE(lua_globals)))
                {
                    color = IM_COL32(221, 198, 122, alpha);
                }
            }
            else if (line[index] == '(' || line[index] == ')' || line[index] == '[' || line[index] == ']')
            {
                color = IM_COL32(177, 177, 181, alpha);
            }
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
            draw_list->AddText(font, font_size, ImVec2(x, pos.y), color, line + index, line + token_end);
            x += font->CalcTextSizeA(font_size, 10000.0f, 0.0f, line + index, line + token_end).x;
            index = token_end;
        }
    }
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
    inline bool code_editor(
        const char* id,
        const ImVec2& min,
        const ImVec2& size,
        char* value,
        size_t value_size,
        float alpha = 1.0f,
        bool enabled = true)
    {
        Context& ctx = context();
        const int a = static_cast<int>(255.0f * clamp01(alpha));
        if (a <= 0)
            return false;
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
        static CodeEditorState state;
        ImFont* font = fallback_font(ctx.text_font);
        const float font_size = 12.0f;
        const float line_height = 15.0f;
        const float number_width = 24.0f;
        const float text_left = min.x + number_width + 6.0f;
        const float content_top = min.y + 8.0f;
        const float content_width = size.x - number_width - 18.0f;
        const float content_height = size.y - 18.0f;
        const ImVec2 max(min.x + size.x, min.y + size.y);
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
        bool changed = false;
        bool active = false;
        bool cursor_keyboard_moved = false;
        ImGuiID input_id = 0;
        if (enabled)
        {
            ImGui::SetNextWindowPos(min);
            ImGui::SetNextWindowSize(size);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
            ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
            ImGui::PushStyleVar(ImGuiStyleVar_ScrollbarSize, 0.0f);
            ImGui::Begin(
                id,
                nullptr,
                ImGuiWindowFlags_NoDecoration |
                ImGuiWindowFlags_NoMove |
                ImGuiWindowFlags_NoSavedSettings |
                ImGuiWindowFlags_NoBackground |
                ImGuiWindowFlags_NoScrollbar |
                ImGuiWindowFlags_NoScrollWithMouse |
                ImGuiWindowFlags_NoBringToFrontOnFocus);
            ImGui::PushFont(font);
            ImGui::SetCursorPos(ImVec2(0.0f, 0.0f));
            if (mouse_in_rect(min, max) && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
            {
                ImGui::FocusWindow(ImGui::GetCurrentWindow());
                ImGui::SetKeyboardFocusHere();
            }
            ImGui::PushStyleColor(ImGuiCol_FrameBg, IM_COL32(0, 0, 0, 0));
            ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(0, 0, 0, 0));
            ImGui::PushStyleColor(ImGuiCol_TextDisabled, IM_COL32(0, 0, 0, 0));
            ImGui::PushStyleColor(ImGuiCol_TextSelectedBg, IM_COL32(0, 0, 0, 0));
            ImGui::PushStyleColor(ImGuiCol_InputTextCursor, IM_COL32(0, 0, 0, 0));
            ImGui::PushStyleColor(ImGuiCol_Border, IM_COL32(0, 0, 0, 0));
            ImGui::PushStyleColor(ImGuiCol_ScrollbarBg, IM_COL32(0, 0, 0, 0));
            ImGui::PushStyleColor(ImGuiCol_ScrollbarGrab, IM_COL32(0, 0, 0, 0));
            ImGui::PushStyleColor(ImGuiCol_ScrollbarGrabHovered, IM_COL32(0, 0, 0, 0));
            ImGui::PushStyleColor(ImGuiCol_ScrollbarGrabActive, IM_COL32(0, 0, 0, 0));
            ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(number_width + 6.0f, 8.0f));
            ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 0.0f);
            ImGui::SetNextItemWidth(size.x);
            ImDrawList* input_draw_list = ImGui::GetWindowDrawList();
            input_draw_list->PushClipRect(ImVec2(-10000.0f, -10000.0f), ImVec2(-9999.0f, -9999.0f), true);
            changed = ImGui::InputTextMultiline(
                "##code_editor_value",
                value,
                value_size,
                size,
                ImGuiInputTextFlags_ReadOnly);
            input_id = ImGui::GetItemID();
            if (mouse_in_rect(min, max) && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
            {
                state.cursor_pos = code_editor_char_from_mouse(
                    value,
                    font,
                    font_size,
                    ImGui::GetIO().MousePos,
                    text_left,
                    content_top,
                    line_height,
                    state.scroll_x,
                    state.scroll_y);
                state.selection_start = state.cursor_pos;
                state.selection_end = state.cursor_pos;
            }
            const int cursor_before_manual_input = state.cursor_pos;
            changed |= manual_text_input(input_id, min, max, value, value_size, state, true);
            if (state.cursor_pos != cursor_before_manual_input && !state.selection_dragging)
                cursor_keyboard_moved = true;
            input_draw_list->PopClipRect();
            active = ImGui::IsItemActive() || manual_text_input_active(input_id);
            if (active)
            {
                int cursor_line_index = 0;
                int cursor_line_start = 0;
                int cursor_line_end = 0;
                code_editor_cursor_line(value, state.cursor_pos, &cursor_line_index, &cursor_line_start, &cursor_line_end);
                const float desired_x = text_width_to(font, font_size, value + cursor_line_start, state.cursor_pos - cursor_line_start);
                if (manual_text_key_pressed(VK_UP, true))
                {
                    state.cursor_pos = code_editor_char_from_line_x(value, font, font_size, cursor_line_index - 1, desired_x);
                    state.selection_anchor = state.cursor_pos;
                    state.selection_start = state.cursor_pos;
                    state.selection_end = state.cursor_pos;
                    cursor_keyboard_moved = true;
                }
                if (manual_text_key_pressed(VK_DOWN, true))
                {
                    state.cursor_pos = code_editor_char_from_line_x(value, font, font_size, cursor_line_index + 1, desired_x);
                    state.selection_anchor = state.cursor_pos;
                    state.selection_start = state.cursor_pos;
                    state.selection_end = state.cursor_pos;
                    cursor_keyboard_moved = true;
                }
            }
            if (active && state.selection_dragging && ImGui::IsMouseDown(ImGuiMouseButton_Left))
            {
                state.cursor_pos = code_editor_char_from_mouse(
                    value,
                    font,
                    font_size,
                    ImGui::GetIO().MousePos,
                    text_left,
                    content_top,
                    line_height,
                    state.scroll_x,
                    state.scroll_y);
                state.selection_start = state.selection_anchor;
                state.selection_end = state.cursor_pos;
            }
            ImGui::PopStyleVar(2);
            ImGui::PopStyleColor(10);
            ImGui::PopFont();
            ImGui::End();
            ImGui::PopStyleVar(3);
        }
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
        int line_count = 1;
        int current_line = 1;
        int line_start = 0;
        float longest_line_width = 0.0f;
        for (int index = 0; value[index] != '\0'; ++index)
        {
            if (index == state.cursor_pos)
                current_line = line_count;
            if (value[index] == '\n')
            {
                const float width = font->CalcTextSizeA(font_size, 10000.0f, 0.0f, value + line_start, value + index).x;
                if (width > longest_line_width)
                    longest_line_width = width;
                ++line_count;
                line_start = index + 1;
            }
        }
        const int text_length = text_length_bytes(value);
        if (state.cursor_pos >= text_length)
            current_line = line_count;
        const float last_width = font->CalcTextSizeA(font_size, 10000.0f, 0.0f, value + line_start, value + text_length).x;
        if (last_width > longest_line_width)
            longest_line_width = last_width;
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
        const float total_height = line_count * line_height + 8.0f;
        const float max_scroll_y = total_height > content_height ? total_height - content_height : 0.0f;
        const float max_scroll_x = longest_line_width > content_width ? longest_line_width - content_width + 12.0f : 0.0f;
        const bool hovered = ImGui::IsMouseHoveringRect(min, max);
        if (enabled && hovered)
            ImGui::SetMouseCursor(ImGuiMouseCursor_TextInput);
        if (enabled && hovered && ImGui::GetIO().MouseWheel != 0.0f)
        {
            if (ImGui::GetIO().KeyShift && max_scroll_x > 0.0f)
            {
                state.scroll_x -= ImGui::GetIO().MouseWheel * 42.0f;
                if (state.scroll_x < 0.0f)
                    state.scroll_x = 0.0f;
                if (state.scroll_x > max_scroll_x)
                    state.scroll_x = max_scroll_x;
            }
            else
                state.scroll_y -= ImGui::GetIO().MouseWheel * line_height * 3.0f;
        }
        if (enabled && hovered && ImGui::GetIO().MouseWheelH != 0.0f && max_scroll_x > 0.0f)
            state.scroll_x -= ImGui::GetIO().MouseWheelH * 42.0f;
        if (state.scroll_y < 0.0f)
            state.scroll_y = 0.0f;
        if (state.scroll_y > max_scroll_y)
            state.scroll_y = max_scroll_y;
        if (state.scroll_x < 0.0f)
            state.scroll_x = 0.0f;
        if (state.scroll_x > max_scroll_x)
            state.scroll_x = max_scroll_x;
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
        const float cursor_line_top = (current_line - 1) * line_height;
        if (active)
        {
            if (cursor_line_top - state.scroll_y < 0.0f)
                state.scroll_y = cursor_line_top;
            if (cursor_line_top + line_height - state.scroll_y > content_height)
                state.scroll_y = cursor_line_top + line_height - content_height;
            if (state.scroll_y > max_scroll_y)
                state.scroll_y = max_scroll_y;
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
            int cursor_line_start_for_scroll = 0;
            for (int index = 0; index < state.cursor_pos && value[index] != '\0'; ++index)
            {
                if (value[index] == '\n')
                    cursor_line_start_for_scroll = index + 1;
            }
            const float cursor_local_x = text_width_to(font, font_size, value + cursor_line_start_for_scroll, state.cursor_pos - cursor_line_start_for_scroll);
            if (cursor_keyboard_moved)
            {
                if (cursor_local_x - state.scroll_x > content_width - 8.0f)
                    state.scroll_x = cursor_local_x - content_width + 8.0f;
                if (cursor_local_x - state.scroll_x < 0.0f)
                    state.scroll_x = cursor_local_x;
                if (state.scroll_x < 0.0f)
                    state.scroll_x = 0.0f;
                if (state.scroll_x > max_scroll_x)
                    state.scroll_x = max_scroll_x;
            }
        }
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
        if (enabled && active && input_id != 0 && hovered && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
        {
            int selection_start = 0;
            int selection_end = 0;
            const int clicked_pos = code_editor_char_from_mouse(
                value,
                font,
                font_size,
                ImGui::GetIO().MousePos,
                text_left,
                content_top,
                line_height,
                state.scroll_x,
                state.scroll_y);
            if (code_editor_word_at(value, clicked_pos, &selection_start, &selection_end))
            {
                if (ImGuiInputTextState* input_state = ImGui::GetInputTextState(input_id))
                {
                    input_state->SetSelection(selection_start, selection_end);
                    input_state->CursorAnimReset();
                }
                state.selection_start = selection_start;
                state.selection_end = selection_end;
                state.cursor_pos = selection_end;
            }
        }
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
        ImDrawList* draw_list = ImGui::GetBackgroundDrawList();
        draw_list->PushClipRect(min, max, true);
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
        const float active_line_y = content_top + (current_line - 1) * line_height - state.scroll_y;
        if (!active && active_line_y >= min.y && active_line_y < max.y - 8.0f)
        {
            draw_list->AddRectFilled(
                ImVec2(min.x + 5.0f, active_line_y - 1.0f),
                ImVec2(max.x - 10.0f, active_line_y + line_height),
                IM_COL32(36, 36, 38, static_cast<int>(150.0f * clamp01(alpha))),
                1.0f);
            draw_list->AddRect(
                ImVec2(min.x + 5.0f, active_line_y - 1.0f),
                ImVec2(max.x - 10.0f, active_line_y + line_height),
                IM_COL32(68, 68, 72, static_cast<int>(190.0f * clamp01(alpha))),
                1.0f);
        }
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
        int selection_a = clamp_text_pos(state.selection_start, text_length);
        int selection_b = clamp_text_pos(state.selection_end, text_length);
        if (selection_a > selection_b)
        {
            const int tmp = selection_a;
            selection_a = selection_b;
            selection_b = tmp;
        }
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
        int render_line = 1;
        int render_line_start = 0;
        for (int index = 0; ; ++index)
        {
            if (value[index] == '\n' || value[index] == '\0')
            {
                const float y = content_top + (render_line - 1) * line_height - state.scroll_y;
                if (y > min.y - line_height && y < max.y - 8.0f)
                {
                    char number_buffer[16];
                    format_positive_int(number_buffer, IM_ARRAYSIZE(number_buffer), render_line);
                    if (state.scroll_x <= 0.5f)
                    {
                        draw_list->AddText(
                            font,
                            font_size,
                            ImVec2(min.x + 10.0f, y),
                            IM_COL32(102, 102, 106, a),
                            number_buffer);
                    }
                    if (active && selection_a != selection_b)
                    {
                        const int line_selection_start = selection_a > render_line_start ? selection_a : render_line_start;
                        const int line_selection_end = selection_b < index ? selection_b : index;
                        if (line_selection_start < line_selection_end)
                        {
                            const float selection_x1 = text_left + text_width_to(font, font_size, value + render_line_start, line_selection_start - render_line_start) - state.scroll_x;
                            const float selection_x2 = text_left + text_width_to(font, font_size, value + render_line_start, line_selection_end - render_line_start) - state.scroll_x;
                            draw_list->AddRectFilled(
                                ImVec2(selection_x1, y + 1.0f),
                                ImVec2(selection_x2, y + line_height - 1.0f),
                                IM_COL32(142, 134, 246, static_cast<int>(90.0f * clamp01(alpha))),
                                1.0f);
                        }
                    }
                    draw_lua_syntax_line(
                        draw_list,
                        font,
                        font_size,
                        value + render_line_start,
                        index - render_line_start,
                        ImVec2(text_left, y),
                        state.scroll_x,
                        a);
                }
                if (value[index] == '\0')
                    break;
                ++render_line;
                render_line_start = index + 1;
            }
        }
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
        if (should_show_caret(ImGui::GetID(id), active, changed, state.cursor_pos))
        {
            int cursor_line_start = 0;
            for (int index = 0; index < state.cursor_pos && value[index] != '\0'; ++index)
            {
                if (value[index] == '\n')
                    cursor_line_start = index + 1;
            }
            const float cursor_x = text_left + text_width_to(font, font_size, value + cursor_line_start, state.cursor_pos - cursor_line_start) - state.scroll_x;
            draw_list->AddRectFilled(
                ImVec2(cursor_x, active_line_y + 1.0f),
                ImVec2(cursor_x + 1.0f, active_line_y + line_height - 1.0f),
                IM_COL32(235, 235, 235, a));
        }
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
        draw_list->PopClipRect();
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
        const ImVec2 h_line_min(min.x + 13.0f, max.y - 6.0f);
        const ImVec2 h_line_max(max.x - 13.0f, h_line_min.y + 2.0f);
        const float horizontal_track_width = h_line_max.x - h_line_min.x;
        draw_list->AddRectFilled(h_line_min, h_line_max, IM_COL32(40, 40, 40, a));
        if (max_scroll_x > 0.0f)
        {
            const float handle_width = horizontal_track_width * (content_width / (content_width + max_scroll_x));
            if (enabled && ImGui::IsMouseHoveringRect(ImVec2(h_line_min.x, h_line_min.y - 5.0f), ImVec2(h_line_max.x, h_line_max.y + 5.0f)) && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
                state.dragging_x = true;
            if (!ImGui::IsMouseDown(ImGuiMouseButton_Left))
                state.dragging_x = false;
            if (state.dragging_x)
            {
                const float track_x = ImGui::GetIO().MousePos.x - h_line_min.x - handle_width * 0.5f;
                const float track_max = horizontal_track_width - handle_width;
                state.scroll_x = track_max > 0.0f ? (track_x / track_max) * max_scroll_x : 0.0f;
                if (state.scroll_x < 0.0f)
                    state.scroll_x = 0.0f;
                if (state.scroll_x > max_scroll_x)
                    state.scroll_x = max_scroll_x;
            }
            const float handle_x = h_line_min.x + (horizontal_track_width - handle_width) * (state.scroll_x / max_scroll_x);
            draw_list->AddRectFilled(
                ImVec2(handle_x, h_line_min.y - 1.0f),
                ImVec2(handle_x + handle_width, h_line_max.y + 1.0f),
                IM_COL32(92, 92, 96, a),
                1.0f);
        }
        if (max_scroll_y > 0.0f)
        {
            const float track_height = size.y - 20.0f;
            const float handle_height = track_height * (content_height / (content_height + max_scroll_y));
            const ImVec2 vertical_track_min(max.x - 10.0f, min.y + 8.0f);
            const ImVec2 vertical_track_max(max.x - 2.0f, min.y + 8.0f + track_height);
            if (enabled && ImGui::IsMouseHoveringRect(vertical_track_min, vertical_track_max) && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
                state.dragging_y = true;
            if (!ImGui::IsMouseDown(ImGuiMouseButton_Left))
                state.dragging_y = false;
            if (state.dragging_y)
            {
                const float track_y = ImGui::GetIO().MousePos.y - vertical_track_min.y - handle_height * 0.5f;
                const float track_max = track_height - handle_height;
                state.scroll_y = track_max > 0.0f ? (track_y / track_max) * max_scroll_y : 0.0f;
                if (state.scroll_y < 0.0f)
                    state.scroll_y = 0.0f;
                if (state.scroll_y > max_scroll_y)
                    state.scroll_y = max_scroll_y;
            }
            const float handle_y = min.y + 8.0f + (track_height - handle_height) * (state.scroll_y / max_scroll_y);
            draw_list->AddRectFilled(
                ImVec2(max.x - 7.0f, min.y + 8.0f),
                ImVec2(max.x - 5.0f, min.y + 8.0f + track_height),
                IM_COL32(40, 40, 40, a));
            draw_list->AddRectFilled(
                ImVec2(max.x - 8.0f, handle_y),
                ImVec2(max.x - 4.0f, handle_y + handle_height),
                IM_COL32(92, 92, 96, a),
                1.0f);
        }
        return changed;
    }
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
    inline bool modal_input(
        const char* id,
        const ImVec2& min,
        const ImVec2& size,
        const char* icon,
        const char* placeholder,
        char* value,
        size_t value_size,
        float alpha,
        float icon_size = 12.0f,
        float text_size = 13.0f)
    {
        Context& ctx = context();
        const float draw_alpha = smooth(alpha);
        if (draw_alpha <= 0.005f)
            return false;
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
        ImFont* text_font = fallback_font(ctx.text_font);
        ImFont* icon_font = fallback_font(ctx.icon_font);
        const int a = static_cast<int>(255.0f * draw_alpha);
        const ImVec2 max(min.x + size.x, min.y + size.y);
        static ModalInputState input_state;
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
        ImGui::SetNextWindowPos(min);
        ImGui::SetNextWindowSize(size);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        ImGui::Begin(
            id,
            nullptr,
            ImGuiWindowFlags_NoDecoration |
            ImGuiWindowFlags_NoMove |
            ImGuiWindowFlags_NoSavedSettings |
            ImGuiWindowFlags_NoBackground |
            ImGuiWindowFlags_NoScrollbar |
            ImGuiWindowFlags_NoScrollWithMouse);
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
        const ImVec2 icon_measure = icon_font->CalcTextSizeA(icon_size, 1000.0f, 0.0f, icon);
        const float center_y = min.y + size.y * 0.5f;
        const float click_text_x = min.x + 38.0f;
        const bool focus_requested = mouse_in_rect(min, max) && ImGui::IsMouseClicked(ImGuiMouseButton_Left);
        if (focus_requested)
        {
            ImGui::FocusWindow(ImGui::GetCurrentWindow());
            ImGui::SetKeyboardFocusHere();
        }
        ImGui::PushFont(text_font);
        ImGui::SetCursorPos(ImVec2(0.0f, 0.0f));
        ImGui::PushStyleColor(ImGuiCol_FrameBg, IM_COL32(0, 0, 0, 0));
        ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(0, 0, 0, 0));
        ImGui::PushStyleColor(ImGuiCol_TextDisabled, IM_COL32(0, 0, 0, 0));
        ImGui::PushStyleColor(ImGuiCol_TextSelectedBg, IM_COL32(0, 0, 0, 0));
        ImGui::PushStyleColor(ImGuiCol_InputTextCursor, IM_COL32(0, 0, 0, 0));
        ImGui::PushStyleColor(ImGuiCol_Border, IM_COL32(0, 0, 0, 0));
        const float input_frame_y = ImMax(0.0f, (size.y - ImGui::GetTextLineHeight()) * 0.5f);
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(38.0f, input_frame_y));
        ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, ImVec2(0.0f, 0.0f));
        ImGui::SetNextItemWidth(size.x);
        ImGui::PushID(id);
        bool changed = ImGui::InputText(
            "##value",
            value,
            value_size,
            ImGuiInputTextFlags_CallbackAlways,
            modal_input_state_callback,
            &input_state);
        const ImGuiID input_id = ImGui::GetItemID();
        if (focus_requested)
        {
            input_state.cursor_pos = text_pos_from_mouse_x(value, text_font, text_size, ImGui::GetIO().MousePos.x, click_text_x, input_state.scroll_x);
            input_state.selection_start = input_state.cursor_pos;
            input_state.selection_end = input_state.cursor_pos;
        }
        changed |= manual_text_input(input_id, min, max, value, value_size, input_state, false);
        const bool active = ImGui::IsItemActive() || manual_text_input_active(input_id);
        const bool hovered = mouse_in_rect(min, max);
        if (active && input_state.selection_dragging && ImGui::IsMouseDown(ImGuiMouseButton_Left))
        {
            input_state.cursor_pos = text_pos_from_mouse_x(value, text_font, text_size, ImGui::GetIO().MousePos.x, click_text_x, input_state.scroll_x);
            input_state.selection_start = input_state.selection_anchor;
            input_state.selection_end = input_state.cursor_pos;
        }
        if (hovered)
            ImGui::SetMouseCursor(ImGuiMouseCursor_TextInput);
        ImGui::PopID();
        ImGui::PopStyleVar(3);
        ImGui::PopStyleColor(6);
        ImGui::PopFont();
        ImGui::End();
        ImGui::PopStyleVar(2);
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
        ImDrawList* draw_list = ImGui::GetForegroundDrawList();
        draw_list->AddRectFilled(min, max, IM_COL32(14, 14, 14, 255), 3.0f);
        draw_list->AddRect(min, max, IM_COL32(31, 31, 31, 255), 3.0f, 0, 1.0f);
        draw_list->AddRect(
            ImVec2(min.x - 1.0f, min.y - 1.0f),
            ImVec2(max.x + 1.0f, max.y + 1.0f),
            IM_COL32(5, 5, 5, 255),
            4.0f,
            0,
            1.0f);
        draw_list->AddText(
            icon_font,
            icon_size,
            ImVec2(min.x + 19.0f, center_y - icon_measure.y * 0.5f),
            IM_COL32(62, 62, 66, a),
            icon);
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
        const float text_x = min.x + 38.0f;
        const float text_width = size.x - 52.0f;
        const ImVec2 text_clip_min(text_x, min.y + 1.0f);
        const ImVec2 text_clip_max(text_x + text_width, max.y - 1.0f);
        const int text_length = text_length_bytes(value);
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
        if (active)
        {
            const int cursor_pos = clamp_text_pos(input_state.cursor_pos, text_length);
            const float cursor_x = text_width_to(text_font, text_size, value, cursor_pos);
            const float total_width = text_width_to(text_font, text_size, value, text_length);
            if (cursor_x - input_state.scroll_x > text_width - 3.0f)
                input_state.scroll_x = cursor_x - text_width + 3.0f;
            if (cursor_x - input_state.scroll_x < 0.0f)
                input_state.scroll_x = cursor_x;
            const float max_scroll = total_width > text_width ? total_width - text_width + 3.0f : 0.0f;
            if (input_state.scroll_x < 0.0f)
                input_state.scroll_x = 0.0f;
            if (input_state.scroll_x > max_scroll)
                input_state.scroll_x = max_scroll;
        }
        else
        {
            input_state.scroll_x = 0.0f;
        }
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
        draw_list->PushClipRect(text_clip_min, text_clip_max, true);
        if (text_length > 0)
        {
            const float text_y = center_y - text_font->CalcTextSizeA(text_size, 1000.0f, 0.0f, value).y * 0.5f;
            if (active && input_state.selection_start != input_state.selection_end)
            {
                int sel_a = clamp_text_pos(input_state.selection_start, text_length);
                int sel_b = clamp_text_pos(input_state.selection_end, text_length);
                if (sel_a > sel_b)
                {
                    const int tmp = sel_a;
                    sel_a = sel_b;
                    sel_b = tmp;
                }
                const float sel_x1 = text_x + text_width_to(text_font, text_size, value, sel_a) - input_state.scroll_x;
                const float sel_x2 = text_x + text_width_to(text_font, text_size, value, sel_b) - input_state.scroll_x;
                draw_list->AddRectFilled(
                    ImVec2(sel_x1, center_y - 9.0f),
                    ImVec2(sel_x2, center_y + 9.0f),
                    IM_COL32(142, 134, 246, static_cast<int>(132.0f * draw_alpha)),
                    1.0f);
            }
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
            draw_list->AddText(
                text_font,
                text_size,
                ImVec2(text_x - input_state.scroll_x, text_y),
                IM_COL32(235, 235, 235, a),
                value);
        }
        else if (!active)
        {
            const ImVec2 placeholder_measure = text_font->CalcTextSizeA(text_size, 1000.0f, 0.0f, placeholder);
            draw_list->AddText(
                text_font,
                text_size,
                ImVec2(min.x + 38.0f, center_y - placeholder_measure.y * 0.5f),
                IM_COL32(74, 74, 78, a),
                placeholder);
        }
        if (should_show_caret(ImGui::GetID(id), active, changed, input_state.cursor_pos))
        {
            const int cursor_pos = clamp_text_pos(input_state.cursor_pos, text_length);
            const float cursor_x = text_x + text_width_to(text_font, text_size, value, cursor_pos) - input_state.scroll_x;
            draw_list->AddRectFilled(
                ImVec2(cursor_x, center_y - 8.0f),
                ImVec2(cursor_x + 1.0f, center_y + 8.0f),
                IM_COL32(235, 235, 235, a));
        }
        draw_list->PopClipRect();
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
        return changed;
    }
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
    inline bool tab(
        const char* id,
        int index,
        int& active_index,
        const char* icon,
        const char* label,
        ImFont* icon_font,
        ImFont* tooltip_font,
        const ImVec2& button_min,
        const ImVec2& button_size,
        const ImVec2& icon_anchor,
        const ImVec2& bar_min,
        const ImVec2& bar_size,
        const ImVec2& tooltip_min,
        float icon_size,
        float& icon_fade,
        float& bar_fade,
        float& hover_fade,
        float delta_time)
    {
        ImGui::SetNextWindowPos(button_min);
        ImGui::SetNextWindowSize(button_size);
        ImGui::Begin(
            id,
            nullptr,
            ImGuiWindowFlags_NoDecoration |
            ImGuiWindowFlags_NoMove |
            ImGuiWindowFlags_NoSavedSettings |
            ImGuiWindowFlags_NoBackground |
            ImGuiWindowFlags_NoScrollbar |
            ImGuiWindowFlags_NoScrollWithMouse);
        ImGui::SetCursorPos(ImVec2(0.0f, 0.0f));
        ImGui::InvisibleButton("##input", button_size);
        const bool hovered = ImGui::IsItemHovered();
        const bool clicked = ImGui::IsItemClicked(ImGuiMouseButton_Left);
        if (clicked)
            active_index = index;
        ImGui::End();
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
        const float active_target = index == active_index ? 1.0f : 0.0f;
        icon_fade += (active_target - icon_fade) * (delta_time * 4.6f > 1.0f ? 1.0f : delta_time * 4.6f);
        bar_fade += (active_target - bar_fade) * (delta_time * 4.6f > 1.0f ? 1.0f : delta_time * 4.6f);
        hover_fade += ((hovered ? 1.0f : 0.0f) - hover_fade) * (delta_time * 7.0f > 1.0f ? 1.0f : delta_time * 7.0f);
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
        ImDrawList* draw_list = ImGui::GetBackgroundDrawList();
        const float bar_alpha_fade = smooth(bar_fade);
        if (bar_alpha_fade > 0.005f)
        {
            const int bar_alpha = static_cast<int>(255.0f * bar_alpha_fade);
            draw_list->AddRectFilled(
                bar_min,
                ImVec2(bar_min.x + bar_size.x, bar_min.y + bar_size.y),
                IM_COL32(142, 134, 246, bar_alpha),
                1.0f);
        }
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
        const ImVec2 icon_measure = icon_font->CalcTextSizeA(icon_size, 1000.0f, 0.0f, icon);
        const float icon_alpha_fade = smooth(icon_fade);
        const int inactive_value = 86;
        const int active_value = 245;
        const int icon_value = lerp_channel(inactive_value, active_value, icon_alpha_fade);
        draw_list->AddText(
            icon_font,
            icon_size,
            ImVec2(icon_anchor.x - icon_measure.x * 0.5f, icon_anchor.y),
            IM_COL32(icon_value, icon_value, icon_value, 255),
            icon);
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
        const float tooltip_fade = smooth(hover_fade);
        if (tooltip_fade > 0.005f)
        {
            const float tooltip_text_size = 13.0f;
            const ImVec2 label_size = tooltip_font->CalcTextSizeA(tooltip_text_size, 1000.0f, 0.0f, label);
            const float content_width = label_size.x + 18.0f;
            const float tooltip_width = index == 0 ? content_width : (content_width > 55.0f ? content_width : 55.0f);
            const float tooltip_height = 25.0f;
            const int tooltip_alpha = static_cast<int>(255.0f * tooltip_fade);
            ImDrawList* foreground = ImGui::GetForegroundDrawList();
            foreground->AddRectFilled(
                tooltip_min,
                ImVec2(tooltip_min.x + tooltip_width, tooltip_min.y + tooltip_height),
                IM_COL32(21, 21, 21, tooltip_alpha),
                3.0f);
            foreground->AddText(
                tooltip_font,
                tooltip_text_size,
                ImVec2(
                    tooltip_min.x + (tooltip_width - label_size.x) * 0.5f,
                    tooltip_min.y + (tooltip_height - label_size.y) * 0.5f),
                IM_COL32(180, 180, 184, tooltip_alpha),
                label);
        }
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
        return clicked;
    }
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
    inline bool tab(
        const char* label,
        const char* icon,
        int index,
        int& active_index,
        const ImVec2& button_min,
        const ImVec2& icon_anchor,
        const ImVec2& bar_min,
        const ImVec2& tooltip_min,
        float& icon_fade,
        float& bar_fade,
        float& hover_fade,
        ImFont* icon_font = nullptr)
    {
        Context& ctx = context();
        ImFont* resolved_icon_font = fallback_font(icon_font ? icon_font : ctx.icon_font);
        return tab(
            label,
            index,
            active_index,
            icon,
            label,
            resolved_icon_font,
            fallback_font(ctx.tooltip_font),
            button_min,
            ImVec2(48.0f, 30.0f),
            icon_anchor,
            bar_min,
            ImVec2(2.0f, 12.0f),
            tooltip_min,
            16.0f,
            icon_fade,
            bar_fade,
            hover_fade,
            ctx.delta_time);
    }
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
    inline float subtab_width(
        const char* icon,
        const char* label,
        ImFont* icon_font,
        ImFont* text_font,
        float icon_size,
        float text_size)
    {
        const ImVec2 icon_measure = icon_font->CalcTextSizeA(icon_size, 1000.0f, 0.0f, icon);
        const ImVec2 label_measure = text_font->CalcTextSizeA(text_size, 1000.0f, 0.0f, label);
        return icon_measure.x + 8.0f + label_measure.x + 14.0f;
    }
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
    inline bool subtab(
        const char* id,
        int index,
        int& active_index,
        const char* icon,
        const char* label,
        ImFont* icon_font,
        ImFont* text_font,
        const ImVec2& min,
        float icon_size,
        float text_size,
        float alpha,
        float& active_fade,
        float& hover_fade,
        float delta_time,
        bool interactive = true)
    {
        const float width = subtab_width(icon, label, icon_font, text_font, icon_size, text_size);
        bool hovered = false;
        bool clicked = false;
        if (interactive)
        {
            ImGui::SetNextWindowPos(ImVec2(min.x - 4.0f, min.y - 6.0f));
            ImGui::SetNextWindowSize(ImVec2(width, 30.0f));
            ImGui::Begin(
                id,
                nullptr,
                ImGuiWindowFlags_NoDecoration |
                ImGuiWindowFlags_NoMove |
                ImGuiWindowFlags_NoSavedSettings |
                ImGuiWindowFlags_NoBackground |
                ImGuiWindowFlags_NoScrollbar |
                ImGuiWindowFlags_NoScrollWithMouse |
                ImGuiWindowFlags_NoBringToFrontOnFocus);
            ImGui::SetCursorPos(ImVec2(0.0f, 0.0f));
            ImGui::InvisibleButton("##input", ImVec2(width, 30.0f));
            hovered = ImGui::IsItemHovered();
            clicked = ImGui::IsItemClicked(ImGuiMouseButton_Left);
            if (clicked)
                active_index = index;
            ImGui::End();
        }
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
        const float active_target = index == active_index ? 1.0f : 0.0f;
        active_fade += (active_target - active_fade) * (delta_time * 4.6f > 1.0f ? 1.0f : delta_time * 4.6f);
        const bool can_hover = index != active_index;
        hover_fade += ((can_hover && hovered ? 1.0f : 0.0f) - hover_fade) * (delta_time * 7.0f > 1.0f ? 1.0f : delta_time * 7.0f);
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
        const float draw_alpha = smooth(alpha);
        if (draw_alpha <= 0.005f)
            return clicked;
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
        const float active = smooth(active_fade);
        const float hover = smooth(hover_fade) * (1.0f - active);
        const int inactive_value = 86;
        const int hover_value = 58;
        const int active_text_value = 235;
        const int base_value = lerp_channel(inactive_value, hover_value, hover);
        const int icon_r = lerp_channel(base_value, 142, active);
        const int icon_g = lerp_channel(base_value, 134, active);
        const int icon_b = lerp_channel(base_value, 246, active);
        const int text_value = lerp_channel(base_value, active_text_value, active);
        const int alpha_channel = static_cast<int>(255.0f * draw_alpha);
        const ImVec2 icon_measure = icon_font->CalcTextSizeA(icon_size, 1000.0f, 0.0f, icon);
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
        ImDrawList* draw_list = ImGui::GetBackgroundDrawList();
        draw_list->AddText(
            icon_font,
            icon_size,
            min,
            IM_COL32(icon_r, icon_g, icon_b, alpha_channel),
            icon);
        draw_list->AddText(
            text_font,
            text_size,
            ImVec2(min.x + icon_measure.x + 8.0f, min.y + 1.0f),
            IM_COL32(text_value, text_value, text_value, alpha_channel),
            label);
        return clicked;
    }
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
    inline float subtab_width(const char* icon, const char* label)
    {
        Context& ctx = context();
        return subtab_width(icon, label, fallback_font(ctx.icon_font), fallback_font(ctx.text_font), 16.0f, 14.0f);
    }
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
    inline bool subtab(
        int index,
        int& active_index,
        const char* icon,
        const char* label,
        const ImVec2& min,
        float alpha,
        float& active_fade,
        float& hover_fade,
        bool interactive = true)
    {
        Context& ctx = context();
        return subtab(
            label,
            index,
            active_index,
            icon,
            label,
            fallback_font(ctx.icon_font),
            fallback_font(ctx.text_font),
            min,
            16.0f,
            14.0f,
            alpha,
            active_fade,
            hover_fade,
            ctx.delta_time,
            interactive);
    }
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
    inline void child(
        const ImVec2& min,
        const ImVec2& size,
        float rounding = 3.0f,
        ImU32 fill_color = IM_COL32(14, 14, 14, 255),
        ImU32 inner_stroke_color = IM_COL32(22, 22, 22, 255),
        ImU32 outer_stroke_color = IM_COL32(5, 5, 5, 255),
        float alpha = 1.0f)
    {
        const float fade = clamp01(alpha);
        const auto apply_alpha = [fade](ImU32 color) -> ImU32
        {
            const int r = (color >> IM_COL32_R_SHIFT) & 0xFF;
            const int g = (color >> IM_COL32_G_SHIFT) & 0xFF;
            const int b = (color >> IM_COL32_B_SHIFT) & 0xFF;
            const int a = static_cast<int>(((color >> IM_COL32_A_SHIFT) & 0xFF) * fade);
            return IM_COL32(r, g, b, a);
        };
        ImDrawList* draw_list = ImGui::GetBackgroundDrawList();
        const ImVec2 max(min.x + size.x, min.y + size.y);
        draw_list->AddRectFilled(min, max, apply_alpha(fill_color), rounding);
        draw_list->AddRect(min, max, apply_alpha(inner_stroke_color), rounding, 0, 1.0f);
        draw_list->AddRect(
            ImVec2(min.x - 1.0f, min.y - 1.0f),
            ImVec2(max.x + 1.0f, max.y + 1.0f),
            apply_alpha(outer_stroke_color),
            rounding + 1.0f,
            0,
            1.0f);
    }
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
    inline void child_header(
        const ImVec2& child_min,
        const ImVec2& child_size,
        const char* icon,
        const char* label,
        ImFont* icon_font,
        ImFont* text_font,
        float icon_size = 13.0f,
        float text_size = 13.0f,
        float alpha = 1.0f)
    {
        const int a = static_cast<int>(255.0f * clamp01(alpha));
        if (a <= 0)
            return;
        ImDrawList* draw_list = ImGui::GetBackgroundDrawList();
        const float line_y = child_min.y + 43.0f;
        const float line_width = 278.0f;
        const ImVec2 icon_text_size = icon_font->CalcTextSizeA(icon_size, 1000.0f, 0.0f, icon);
        const ImVec2 label_size = text_font->CalcTextSizeA(text_size, 1000.0f, 0.0f, label);
        const float center_y = child_min.y + 21.5f;
        const float icon_x = child_min.x + 21.0f;
        const float text_x = icon_x + icon_text_size.x + 12.0f;
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
        draw_list->AddText(
            icon_font,
            icon_size,
            ImVec2(icon_x, center_y - icon_text_size.y * 0.5f),
            IM_COL32(142, 134, 246, a),
            icon);
        draw_list->AddText(
            text_font,
            text_size,
            ImVec2(text_x, center_y - label_size.y * 0.5f),
            IM_COL32(235, 235, 235, a),
            label);
        draw_list->AddRectFilled(
            ImVec2(child_min.x + child_size.x - line_width, line_y),
            ImVec2(child_min.x + child_size.x, line_y + 1.0f),
            IM_COL32(22, 22, 22, a));
    }
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
    inline void child_header(const ImVec2& child_min, const ImVec2& child_size, const char* icon, const char* label, float alpha = 1.0f)
    {
        Context& ctx = context();
        child_header(child_min, child_size, icon, label, fallback_font(ctx.icon_font), fallback_font(ctx.text_font), 13.0f, 13.0f, alpha);
    }
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
    inline bool button(
        const char* label,
        const ImVec2& min,
        const ImVec2& size,
        ImFont* font,
        float text_size,
        float& hover_fade,
        float delta_time,
        float alpha = 1.0f,
        bool enabled = true)
    {
        bool hovered = false;
        bool clicked = false;
        if (enabled)
        {
            const ImVec2 max(min.x + size.x, min.y + size.y);
            hovered = mouse_in_rect(min, max);
            clicked = hovered && ImGui::IsMouseClicked(ImGuiMouseButton_Left);
            if (hovered)
                ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
        }
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
        ImDrawList* draw_list = ImGui::GetBackgroundDrawList();
        const ImVec2 max(min.x + size.x, min.y + size.y);
        const float target_hover = hovered ? 1.0f : 0.0f;
        hover_fade += (target_hover - hover_fade) * (delta_time * 8.0f > 1.0f ? 1.0f : delta_time * 8.0f);
        const float fade = smooth(hover_fade);
        const int r = lerp_channel(148, 108, fade);
        const int g = lerp_channel(140, 102, fade);
        const int b = lerp_channel(246, 198, fade);
        const int a = static_cast<int>(255.0f * clamp01(alpha));
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
        draw_list->AddRectFilled(min, max, IM_COL32(r, g, b, a), 2.0f);
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
        const ImVec2 text_size_vec = font->CalcTextSizeA(text_size, 1000.0f, 0.0f, label);
        draw_list->AddText(
            font,
            text_size,
            ImVec2(
                min.x + (size.x - text_size_vec.x) * 0.5f,
                min.y + (size.y - text_size_vec.y) * 0.5f),
            IM_COL32(0, 0, 0, a),
            label);
        return clicked;
    }
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
    inline bool button(const char* label, const ImVec2& min, const ImVec2& size, float& hover_fade, float text_size = 12.0f, float alpha = 1.0f, bool enabled = true)
    {
        Context& ctx = context();
        return button(label, min, size, fallback_font(ctx.text_font), text_size, hover_fade, ctx.delta_time, alpha, enabled);
    }
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
    inline bool button_icon(
        const char* label,
        const char* icon,
        const ImVec2& min,
        const ImVec2& size,
        ImFont* text_font,
        ImFont* icon_font,
        float text_size,
        float icon_size,
        float& hover_fade,
        float delta_time,
        float alpha = 1.0f,
        bool enabled = true)
    {
        bool hovered = false;
        bool clicked = false;
        if (enabled)
        {
            const ImVec2 max(min.x + size.x, min.y + size.y);
            hovered = mouse_in_rect(min, max);
            clicked = hovered && ImGui::IsMouseClicked(ImGuiMouseButton_Left);
            if (hovered)
                ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
        }
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
        ImDrawList* draw_list = ImGui::GetBackgroundDrawList();
        const ImVec2 max(min.x + size.x, min.y + size.y);
        const float target_hover = hovered ? 1.0f : 0.0f;
        hover_fade += (target_hover - hover_fade) * (delta_time * 8.0f > 1.0f ? 1.0f : delta_time * 8.0f);
        const float fade = smooth(hover_fade);
        const int r = lerp_channel(148, 108, fade);
        const int g = lerp_channel(140, 102, fade);
        const int b = lerp_channel(246, 198, fade);
        const int a = static_cast<int>(255.0f * clamp01(alpha));
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
        draw_list->AddRectFilled(min, max, IM_COL32(r, g, b, a), 2.0f);
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
        const ImVec2 icon_measure = icon_font->CalcTextSizeA(icon_size, 1000.0f, 0.0f, icon);
        const ImVec2 text_measure = text_font->CalcTextSizeA(text_size, 1000.0f, 0.0f, label);
        const float gap = 12.0f;
        const float content_width = icon_measure.x + gap + text_measure.x;
        const float start_x = min.x + (size.x - content_width) * 0.5f;
        const float center_y = min.y + size.y * 0.5f;
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
        draw_list->AddText(
            icon_font,
            icon_size,
            ImVec2(start_x + 3.0f, center_y - icon_measure.y * 0.5f),
            IM_COL32(0, 0, 0, a),
            icon);
        draw_list->AddText(
            text_font,
            text_size,
            ImVec2(start_x + icon_measure.x + gap, center_y - text_measure.y * 0.5f),
            IM_COL32(0, 0, 0, a),
            label);
        return clicked;
    }
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
    inline bool button(
        const char* label,
        const char* icon,
        const ImVec2& min,
        const ImVec2& size,
        float& hover_fade,
        float text_size = 12.0f,
        float icon_size = 14.0f,
        float alpha = 1.0f,
        bool enabled = true)
    {
        Context& ctx = context();
        return button_icon(
            label,
            icon,
            min,
            size,
            fallback_font(ctx.text_font),
            fallback_font(ctx.icon_font),
            text_size,
            icon_size,
            hover_fade,
            ctx.delta_time,
            alpha,
            enabled);
    }
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
    inline void disabled_button(
        const char* label,
        const char* icon,
        const ImVec2& min,
        const ImVec2& size,
        float text_size = 12.0f,
        float icon_size = 14.0f,
        float alpha = 1.0f)
    {
        Context& ctx = context();
        ImDrawList* draw_list = ImGui::GetBackgroundDrawList();
        ImFont* text_font = fallback_font(ctx.text_font);
        ImFont* icon_font = fallback_font(ctx.icon_font);
        const ImVec2 max(min.x + size.x, min.y + size.y);
        const int a = static_cast<int>(255.0f * clamp01(alpha));
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
        draw_list->AddRectFilled(min, max, IM_COL32(105, 99, 170, a), 2.0f);
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
        const ImVec2 icon_measure = icon_font->CalcTextSizeA(icon_size, 1000.0f, 0.0f, icon);
        const ImVec2 text_measure = text_font->CalcTextSizeA(text_size, 1000.0f, 0.0f, label);
        const float gap = 12.0f;
        const float content_width = icon_measure.x + gap + text_measure.x;
        const float start_x = min.x + (size.x - content_width) * 0.5f;
        const float center_y = min.y + size.y * 0.5f;
        const int content_alpha = static_cast<int>(170.0f * clamp01(alpha));
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
        draw_list->AddText(
            icon_font,
            icon_size,
            ImVec2(start_x + 3.0f, center_y - icon_measure.y * 0.5f),
            IM_COL32(18, 18, 24, content_alpha),
            icon);
        draw_list->AddText(
            text_font,
            text_size,
            ImVec2(start_x + icon_measure.x + gap, center_y - text_measure.y * 0.5f),
            IM_COL32(18, 18, 24, content_alpha),
            label);
    }
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
    inline bool icon_button(
        const char* id,
        const char* icon,
        const ImVec2& min,
        const ImVec2& size,
        float& hover_fade,
        float icon_size = 14.0f,
        float alpha = 1.0f,
        bool enabled = true)
    {
        Context& ctx = context();
        bool hovered = false;
        bool clicked = false;
        if (enabled)
        {
            const ImVec2 max(min.x + size.x, min.y + size.y);
            hovered = mouse_in_rect(min, max);
            clicked = hovered && ImGui::IsMouseClicked(ImGuiMouseButton_Left);
            if (hovered)
                ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
        }
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
        ImDrawList* draw_list = ImGui::GetBackgroundDrawList();
        const ImVec2 max(min.x + size.x, min.y + size.y);
        hover_fade += ((hovered ? 1.0f : 0.0f) - hover_fade) * (ctx.delta_time * 8.0f > 1.0f ? 1.0f : ctx.delta_time * 8.0f);
        const float fade = smooth(hover_fade);
        const int r = lerp_channel(148, 108, fade);
        const int g = lerp_channel(140, 102, fade);
        const int b = lerp_channel(246, 198, fade);
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
        const int draw_alpha = static_cast<int>(255.0f * clamp01(alpha));
        draw_list->AddRectFilled(min, max, IM_COL32(r, g, b, draw_alpha), 2.0f);
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
        ImFont* resolved_icon_font = fallback_font(ctx.icon_font);
        const ImVec2 icon_measure = resolved_icon_font->CalcTextSizeA(icon_size, 1000.0f, 0.0f, icon);
        draw_list->AddText(
            resolved_icon_font,
            icon_size,
            ImVec2(min.x + (size.x - icon_measure.x) * 0.5f, min.y + (size.y - icon_measure.y) * 0.5f),
            IM_COL32(0, 0, 0, draw_alpha),
            icon);
        return clicked;
    }
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
    inline bool icon_button(
        const char* icon,
        const ImVec2& min,
        const ImVec2& size,
        float& hover_fade,
        float icon_size = 14.0f,
        float alpha = 1.0f,
        bool enabled = true)
    {
        return icon_button(icon, icon, min, size, hover_fade, icon_size, alpha, enabled);
    }
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
    inline bool icon_action(
        const char* id,
        const char* icon,
        const ImVec2& center,
        const ImVec2& hit_size,
        ImFont* icon_font,
        float icon_size,
        float& hover_fade,
        float delta_time,
        float alpha = 1.0f,
        bool enabled = true)
    {
        const ImVec2 min(center.x - hit_size.x * 0.5f, center.y - hit_size.y * 0.5f);
        bool hovered = false;
        bool clicked = false;
        if (enabled)
        {
            const ImVec2 max(min.x + hit_size.x, min.y + hit_size.y);
            hovered = mouse_in_rect(min, max);
            clicked = hovered && ImGui::IsMouseClicked(ImGuiMouseButton_Left);
            if (hovered)
                ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
        }
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
        hover_fade += ((hovered ? 1.0f : 0.0f) - hover_fade) * (delta_time * 8.0f > 1.0f ? 1.0f : delta_time * 8.0f);
        const float fade = smooth(hover_fade);
        const int value = lerp_channel(62, 132, fade);
        const ImVec2 icon_measure = icon_font->CalcTextSizeA(icon_size, 1000.0f, 0.0f, icon);
        const int a = static_cast<int>(255.0f * clamp01(alpha));
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
        ImGui::GetBackgroundDrawList()->AddText(
            icon_font,
            icon_size,
            ImVec2(center.x - icon_measure.x * 0.5f, center.y - icon_measure.y * 0.5f),
            IM_COL32(value, value, value + 4, a),
            icon);
        return clicked;
    }
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
    inline bool icon_action(const char* id, const char* icon, const ImVec2& center, float& hover_fade, float alpha = 1.0f, bool enabled = true)
    {
        Context& ctx = context();
        return icon_action(id, icon, center, ImVec2(28.0f, 30.0f), fallback_font(ctx.icon_font), 14.0f, hover_fade, ctx.delta_time, alpha, enabled);
    }
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
    inline void search_box(
        const ImVec2& min,
        const ImVec2& size,
        const char* icon,
        const char* placeholder,
        ImFont* icon_font,
        ImFont* text_font,
        float icon_size = 14.0f,
        float text_size = 13.0f)
    {
        ImDrawList* draw_list = ImGui::GetBackgroundDrawList();
        const ImVec2 max(min.x + size.x, min.y + size.y);
        draw_list->AddRectFilled(min, max, IM_COL32(14, 14, 14, 255), 3.0f);
        draw_list->AddRect(min, max, IM_COL32(31, 31, 31, 255), 3.0f, 0, 1.0f);
        draw_list->AddRect(
            ImVec2(min.x - 1.0f, min.y - 1.0f),
            ImVec2(max.x + 1.0f, max.y + 1.0f),
            IM_COL32(8, 8, 8, 255),
            4.0f,
            0,
            1.0f);
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
        const ImVec2 icon_measure = icon_font->CalcTextSizeA(icon_size, 1000.0f, 0.0f, icon);
        const ImVec2 text_measure = text_font->CalcTextSizeA(text_size, 1000.0f, 0.0f, placeholder);
        const float center_y = min.y + size.y * 0.5f;
        const float icon_x = min.x + 18.0f;
        const float text_x = icon_x + icon_measure.x + 13.0f;
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
        draw_list->AddText(
            icon_font,
            icon_size,
            ImVec2(icon_x, center_y - icon_measure.y * 0.5f),
            IM_COL32(62, 62, 66, 255),
            icon);
        draw_list->AddText(
            text_font,
            text_size,
            ImVec2(text_x, center_y - text_measure.y * 0.5f),
            IM_COL32(74, 74, 78, 255),
            placeholder);
    }
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
    inline void search_box(
        const ImVec2& min,
        const ImVec2& size,
        const char* placeholder = "Search for config...",
        const char* icon = u8"\uf5ae")
    {
        Context& ctx = context();
        search_box(min, size, icon, placeholder, fallback_font(ctx.icon_font), fallback_font(ctx.text_font));
    }
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
    inline bool search_box_input(
        const char* id,
        const ImVec2& min,
        const ImVec2& size,
        char* value,
        size_t value_size,
        const char* placeholder = "Search for config...",
        const char* icon = u8"\uf5ae",
        float alpha = 1.0f,
        bool enabled = true,
        int fill_r = 14,
        int fill_g = 14,
        int fill_b = 14)
    {
        Context& ctx = context();
        const int a = static_cast<int>(255.0f * clamp01(alpha));
        if (a <= 0)
            return false;
        ImFont* icon_font = fallback_font(ctx.icon_font);
        ImFont* text_font = fallback_font(ctx.text_font);
        const float icon_size = 14.0f;
        const float text_size = 13.0f;
        const ImVec2 max(min.x + size.x, min.y + size.y);
        const float center_y = min.y + size.y * 0.5f;
        const float icon_x = min.x + 18.0f;
        const ImVec2 icon_measure = icon_font->CalcTextSizeA(icon_size, 1000.0f, 0.0f, icon);
        const float text_x = icon_x + icon_measure.x + 13.0f;
        const float text_width = size.x - (text_x - min.x) - 12.0f;
        static ModalInputState search_state;
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
        bool changed = false;
        bool active = false;
        if (enabled)
        {
            ImGui::SetNextWindowPos(min);
            ImGui::SetNextWindowSize(size);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
            ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
            ImGui::Begin(
                id,
                nullptr,
                ImGuiWindowFlags_NoDecoration |
                ImGuiWindowFlags_NoMove |
                ImGuiWindowFlags_NoSavedSettings |
                ImGuiWindowFlags_NoBackground |
                ImGuiWindowFlags_NoScrollbar |
                ImGuiWindowFlags_NoScrollWithMouse);
            ImGui::PushFont(text_font);
            if (mouse_in_rect(min, max) && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
            {
                ImGui::FocusWindow(ImGui::GetCurrentWindow());
                ImGui::SetKeyboardFocusHere();
            }
            ImGui::SetCursorPos(ImVec2(0.0f, 0.0f));
            ImGui::PushStyleColor(ImGuiCol_FrameBg, IM_COL32(0, 0, 0, 0));
            ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(0, 0, 0, 0));
            ImGui::PushStyleColor(ImGuiCol_TextDisabled, IM_COL32(0, 0, 0, 0));
            ImGui::PushStyleColor(ImGuiCol_TextSelectedBg, IM_COL32(0, 0, 0, 0));
            ImGui::PushStyleColor(ImGuiCol_InputTextCursor, IM_COL32(0, 0, 0, 0));
            ImGui::PushStyleColor(ImGuiCol_Border, IM_COL32(0, 0, 0, 0));
            const float input_frame_y = ImMax(0.0f, (size.y - ImGui::GetTextLineHeight()) * 0.5f);
            ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(text_x - min.x, input_frame_y));
            ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 0.0f);
            ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, ImVec2(0.0f, 0.0f));
            ImGui::SetNextItemWidth(size.x);
            changed = ImGui::InputText(
                "##value",
                value,
                value_size,
                ImGuiInputTextFlags_CallbackAlways,
                modal_input_state_callback,
                &search_state);
            const ImGuiID input_id = ImGui::GetItemID();
            if (mouse_in_rect(min, max) && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
            {
                search_state.cursor_pos = text_pos_from_mouse_x(value, text_font, text_size, ImGui::GetIO().MousePos.x, text_x, search_state.scroll_x);
                search_state.selection_start = search_state.cursor_pos;
                search_state.selection_end = search_state.cursor_pos;
            }
            changed |= manual_text_input(input_id, min, max, value, value_size, search_state, false);
            active = ImGui::IsItemActive() || manual_text_input_active(input_id);
            const bool hovered = mouse_in_rect(min, max);
            if (active && search_state.selection_dragging && ImGui::IsMouseDown(ImGuiMouseButton_Left))
            {
                search_state.cursor_pos = text_pos_from_mouse_x(value, text_font, text_size, ImGui::GetIO().MousePos.x, text_x, search_state.scroll_x);
                search_state.selection_start = search_state.selection_anchor;
                search_state.selection_end = search_state.cursor_pos;
            }
            if (hovered)
                ImGui::SetMouseCursor(ImGuiMouseCursor_TextInput);
            ImGui::PopStyleVar(3);
            ImGui::PopStyleColor(6);
            ImGui::PopFont();
            ImGui::End();
            ImGui::PopStyleVar(2);
        }
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
        ImDrawList* draw_list = ImGui::GetBackgroundDrawList();
        draw_list->AddRectFilled(min, max, IM_COL32(fill_r, fill_g, fill_b, a), 3.0f);
        draw_list->AddRect(min, max, IM_COL32(31, 31, 31, a), 3.0f, 0, 1.0f);
        draw_list->AddRect(
            ImVec2(min.x - 1.0f, min.y - 1.0f),
            ImVec2(max.x + 1.0f, max.y + 1.0f),
            IM_COL32(8, 8, 8, a),
            4.0f,
            0,
            1.0f);
        draw_list->AddText(
            icon_font,
            icon_size,
            ImVec2(icon_x, center_y - icon_measure.y * 0.5f),
            IM_COL32(62, 62, 66, a),
            icon);
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
        const ImVec2 text_clip_min(text_x, min.y + 1.0f);
        const ImVec2 text_clip_max(text_x + text_width, max.y - 1.0f);
        const int text_length = text_length_bytes(value);
        if (active)
        {
            const int cursor_pos = clamp_text_pos(search_state.cursor_pos, text_length);
            const float cursor_x = text_width_to(text_font, text_size, value, cursor_pos);
            const float total_width = text_width_to(text_font, text_size, value, text_length);
            if (cursor_x - search_state.scroll_x > text_width - 3.0f)
                search_state.scroll_x = cursor_x - text_width + 3.0f;
            if (cursor_x - search_state.scroll_x < 0.0f)
                search_state.scroll_x = cursor_x;
            const float max_scroll = total_width > text_width ? total_width - text_width + 3.0f : 0.0f;
            if (search_state.scroll_x < 0.0f)
                search_state.scroll_x = 0.0f;
            if (search_state.scroll_x > max_scroll)
                search_state.scroll_x = max_scroll;
        }
        else
        {
            search_state.scroll_x = 0.0f;
        }
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
        draw_list->PushClipRect(text_clip_min, text_clip_max, true);
        if (text_length > 0)
        {
            const float text_y = center_y - text_font->CalcTextSizeA(text_size, 1000.0f, 0.0f, value).y * 0.5f;
            if (active && search_state.selection_start != search_state.selection_end)
            {
                int sel_a = clamp_text_pos(search_state.selection_start, text_length);
                int sel_b = clamp_text_pos(search_state.selection_end, text_length);
                if (sel_a > sel_b)
                {
                    const int tmp = sel_a;
                    sel_a = sel_b;
                    sel_b = tmp;
                }
                const float sel_x1 = text_x + text_width_to(text_font, text_size, value, sel_a) - search_state.scroll_x;
                const float sel_x2 = text_x + text_width_to(text_font, text_size, value, sel_b) - search_state.scroll_x;
                draw_list->AddRectFilled(
                    ImVec2(sel_x1, center_y - 9.0f),
                    ImVec2(sel_x2, center_y + 9.0f),
                    IM_COL32(142, 134, 246, static_cast<int>(132.0f * clamp01(alpha))),
                    1.0f);
            }
            draw_list->AddText(
                text_font,
                text_size,
                ImVec2(text_x - search_state.scroll_x, text_y),
                IM_COL32(210, 210, 214, a),
                value);
        }
        else
        {
            const ImVec2 placeholder_measure = text_font->CalcTextSizeA(text_size, 1000.0f, 0.0f, placeholder);
            draw_list->AddText(
                text_font,
                text_size,
                ImVec2(text_x, center_y - placeholder_measure.y * 0.5f),
                IM_COL32(74, 74, 78, a),
                placeholder);
        }
        if (should_show_caret(ImGui::GetID(id), active, changed, search_state.cursor_pos))
        {
            const int cursor_pos = clamp_text_pos(search_state.cursor_pos, text_length);
            const float cursor_x = text_x + text_width_to(text_font, text_size, value, cursor_pos) - search_state.scroll_x;
            draw_list->AddRectFilled(
                ImVec2(cursor_x, center_y - 8.0f),
                ImVec2(cursor_x + 1.0f, center_y + 8.0f),
                IM_COL32(235, 235, 235, a));
        }
        draw_list->PopClipRect();
        return changed;
    }
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
    inline void configbox(const ImVec2& min, const ImVec2& size, float alpha = 1.0f)
    {
        ImDrawList* draw_list = ImGui::GetBackgroundDrawList();
        const ImVec2 max(min.x + size.x, min.y + size.y);
        const int a = static_cast<int>(255.0f * clamp01(alpha));
        draw_list->AddRectFilled(min, max, IM_COL32(14, 14, 14, a), 3.0f);
        draw_list->AddRect(min, max, IM_COL32(16, 16, 16, a), 3.0f, 0, 1.0f);
        draw_list->AddRect(
            ImVec2(min.x - 1.0f, min.y - 1.0f),
            ImVec2(max.x + 1.0f, max.y + 1.0f),
            IM_COL32(5, 5, 5, a),
            4.0f,
            0,
            1.0f);
    }
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
    inline bool config_name_input(
        const char* id,
        const ImVec2& min,
        float width,
        char* value,
        size_t value_size,
        bool& editing,
        float& underline_fade,
        float alpha = 1.0f,
        bool enabled = true)
    {
        Context& ctx = context();
        ImFont* text_font = fallback_font(ctx.text_font);
        ImFont* icon_font = fallback_font(ctx.icon_font);
        const float text_size = 13.0f;
        const float icon_size = 14.0f;
        const ImVec2 hit_size(width, 24.0f);
        bool changed = false;
        static ModalInputState config_name_state;
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
        if (enabled)
        {
            ImGui::SetNextWindowPos(min);
            ImGui::SetNextWindowSize(hit_size);
            ImGui::Begin(
                id,
                nullptr,
                ImGuiWindowFlags_NoDecoration |
                ImGuiWindowFlags_NoMove |
                ImGuiWindowFlags_NoSavedSettings |
                ImGuiWindowFlags_NoBackground |
                ImGuiWindowFlags_NoScrollbar |
                ImGuiWindowFlags_NoScrollWithMouse);
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
            if (editing)
            {
                ImGui::SetCursorPos(ImVec2(0.0f, 0.0f));
                ImGui::PushFont(text_font);
                ImGui::PushStyleColor(ImGuiCol_FrameBg, IM_COL32(0, 0, 0, 0));
                ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(235, 235, 235, 255));
                ImGui::PushStyleColor(ImGuiCol_Border, IM_COL32(0, 0, 0, 0));
                ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0.0f, 0.0f));
                ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 0.0f);
                ImGui::SetNextItemWidth(width - 24.0f);
                if (!ImGui::IsAnyItemActive())
                    ImGui::SetKeyboardFocusHere();
                if (ImGui::InputText("##config_name_value", value, value_size, ImGuiInputTextFlags_EnterReturnsTrue))
                    editing = false;
                const ImGuiID input_id = ImGui::GetItemID();
                const ImVec2 hit_max(min.x + hit_size.x, min.y + hit_size.y);
                if (mouse_in_rect(min, hit_max) && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
                {
                    config_name_state.cursor_pos = text_pos_from_mouse_x(value, text_font, text_size, ImGui::GetIO().MousePos.x, min.x, config_name_state.scroll_x);
                    config_name_state.selection_start = config_name_state.cursor_pos;
                    config_name_state.selection_end = config_name_state.cursor_pos;
                }
                changed = ImGui::IsItemEdited();
                changed |= manual_text_input(input_id, min, hit_max, value, value_size, config_name_state, false);
                const bool active = ImGui::IsItemActive() || manual_text_input_active(input_id);
                if (active && config_name_state.selection_dragging && ImGui::IsMouseDown(ImGuiMouseButton_Left))
                {
                    config_name_state.cursor_pos = text_pos_from_mouse_x(value, text_font, text_size, ImGui::GetIO().MousePos.x, min.x, config_name_state.scroll_x);
                    config_name_state.selection_start = config_name_state.selection_anchor;
                    config_name_state.selection_end = config_name_state.cursor_pos;
                }
                if (mouse_in_rect(min, hit_max))
                    ImGui::SetMouseCursor(ImGuiMouseCursor_TextInput);
                if (!active && ImGui::IsMouseClicked(ImGuiMouseButton_Left) && !mouse_in_rect(min, hit_max))
                    editing = false;
                ImGui::PopStyleVar(2);
                ImGui::PopStyleColor(3);
                ImGui::PopFont();
            }
            else
            {
                ImGui::SetCursorPos(ImVec2(0.0f, 0.0f));
                ImGui::InvisibleButton("##config_name_hit", hit_size);
                if (ImGui::IsItemClicked(ImGuiMouseButton_Left))
                {
                    editing = true;
                }
            }
            ImGui::End();
        }
        else
        {
            editing = false;
        }
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
        ImDrawList* draw_list = ImGui::GetBackgroundDrawList();
        const int draw_alpha = static_cast<int>(255.0f * clamp01(alpha));
        const ImVec2 text_measure = text_font->CalcTextSizeA(text_size, 1000.0f, 0.0f, value);
        if (!editing || !enabled)
        {
            draw_list->AddText(text_font, text_size, min, IM_COL32(235, 235, 235, draw_alpha), value);
        }
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
        const char* edit_icon = u8"\uf18c";
        const ImVec2 icon_measure = icon_font->CalcTextSizeA(icon_size, 1000.0f, 0.0f, edit_icon);
        float icon_x = min.x + text_measure.x + 10.0f;
        const float max_icon_x = min.x + width - icon_measure.x;
        if (icon_x > max_icon_x)
            icon_x = max_icon_x;
        const ImVec2 icon_pos(icon_x, min.y + 1.0f);
        draw_list->AddText(icon_font, icon_size, icon_pos, IM_COL32(88, 88, 92, draw_alpha), edit_icon);
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
        underline_fade += ((editing ? 1.0f : 0.0f) - underline_fade) * (ctx.delta_time * 10.0f > 1.0f ? 1.0f : ctx.delta_time * 10.0f);
        const float underline = smooth(underline_fade);
        if (underline > 0.005f)
        {
            const int alpha_channel = static_cast<int>(230.0f * underline * clamp01(alpha));
            const float underline_width = text_measure.x + 2.0f;
            draw_list->AddRectFilled(
                ImVec2(min.x, min.y + text_measure.y + 3.0f),
                ImVec2(min.x + underline_width, min.y + text_measure.y + 4.0f),
                IM_COL32(235, 235, 235, alpha_channel));
        }
        return changed;
    }
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
    inline void config_date(const ImVec2& min, const char* value, float alpha = 1.0f)
    {
        Context& ctx = context();
        ImGui::GetBackgroundDrawList()->AddText(
            fallback_font(ctx.text_font),
            13.0f,
            min,
            IM_COL32(62, 62, 66, static_cast<int>(255.0f * clamp01(alpha))),
            value);
    }
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
    inline bool icon_action_tinted(
        const char* id,
        const char* icon,
        const ImVec2& center,
        const ImVec2& hit_size,
        ImU32 base_color,
        ImU32 hover_color,
        float& hover_fade,
        float icon_size = 14.0f,
        float alpha = 1.0f,
        bool enabled = true)
    {
        Context& ctx = context();
        const ImVec2 min(center.x - hit_size.x * 0.5f, center.y - hit_size.y * 0.5f);
        bool hovered = false;
        bool clicked = false;
        if (enabled)
        {
            const ImVec2 max(min.x + hit_size.x, min.y + hit_size.y);
            hovered = mouse_in_rect(min, max);
            clicked = hovered && ImGui::IsMouseClicked(ImGuiMouseButton_Left);
            if (hovered)
                ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
        }
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
        hover_fade += ((hovered ? 1.0f : 0.0f) - hover_fade) * (ctx.delta_time * 8.0f > 1.0f ? 1.0f : ctx.delta_time * 8.0f);
        const float fade = smooth(hover_fade);
        const int base_r = (base_color >> IM_COL32_R_SHIFT) & 0xFF;
        const int base_g = (base_color >> IM_COL32_G_SHIFT) & 0xFF;
        const int base_b = (base_color >> IM_COL32_B_SHIFT) & 0xFF;
        const int hover_r = (hover_color >> IM_COL32_R_SHIFT) & 0xFF;
        const int hover_g = (hover_color >> IM_COL32_G_SHIFT) & 0xFF;
        const int hover_b = (hover_color >> IM_COL32_B_SHIFT) & 0xFF;
        ImFont* icon_font = fallback_font(ctx.icon_font);
        const ImVec2 icon_measure = icon_font->CalcTextSizeA(icon_size, 1000.0f, 0.0f, icon);
        ImGui::GetBackgroundDrawList()->AddText(
            icon_font,
            icon_size,
            ImVec2(center.x - icon_measure.x * 0.5f, center.y - icon_measure.y * 0.5f),
            IM_COL32(
                lerp_channel(base_r, hover_r, fade),
                lerp_channel(base_g, hover_g, fade),
                lerp_channel(base_b, hover_b, fade),
                static_cast<int>(255.0f * clamp01(alpha))),
            icon);
        return clicked;
    }
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
    inline bool configbox_actions(
        const char* trash_id,
        const char* copy_id,
        const ImVec2& box_min,
        float& trash_hover,
        float& copy_hover,
        float alpha = 1.0f,
        bool enabled = true)
    {
        ImDrawList* draw_list = ImGui::GetBackgroundDrawList();
        const float action_y = box_min.y + 82.0f;
        const bool trash_clicked = icon_action_tinted(
            trash_id,
            u8"\uf182",
            ImVec2(box_min.x + 24.0f, action_y),
            ImVec2(28.0f, 30.0f),
            IM_COL32(170, 92, 92, 255),
            IM_COL32(215, 126, 126, 255),
            trash_hover,
            14.0f,
            alpha,
            enabled);
        draw_list->AddRectFilled(
            ImVec2(box_min.x + 42.0f, action_y - 4.0f),
            ImVec2(box_min.x + 43.0f, action_y + 4.0f),
            IM_COL32(16, 16, 16, static_cast<int>(255.0f * clamp01(alpha))));
        icon_action_tinted(
            copy_id,
            u8"\uf41c",
            ImVec2(box_min.x + 59.0f, action_y),
            ImVec2(28.0f, 30.0f),
            IM_COL32(170, 174, 174, 255),
            IM_COL32(215, 218, 218, 255),
            copy_hover,
            14.0f,
            alpha,
            enabled);
        return trash_clicked;
    }
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
    inline bool combobox(
        const char* value,
        const ImVec2& min,
        const ImVec2& size,
        ImFont* text_font,
        ImFont* icon_font,
        float text_size = 13.0f,
        float arrow_size = 11.0f,
        float alpha = 1.0f,
        bool enabled = true)
    {
        const int a = static_cast<int>(255.0f * clamp01(alpha));
        if (a <= 0)
            return false;
        ImDrawList* draw_list = ImGui::GetBackgroundDrawList();
        const ImVec2 max(min.x + size.x, min.y + size.y);
        draw_list->AddRectFilled(min, max, IM_COL32(24, 24, 24, a), 3.0f);
        draw_list->AddRect(min, max, IM_COL32(31, 31, 31, a), 3.0f, 0, 1.0f);
        draw_list->AddRect(
            ImVec2(min.x - 1.0f, min.y - 1.0f),
            ImVec2(max.x + 1.0f, max.y + 1.0f),
            IM_COL32(8, 8, 8, a),
            4.0f,
            0,
            1.0f);
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
        const ImVec2 value_size = text_font->CalcTextSizeA(text_size, 1000.0f, 0.0f, value);
        draw_list->AddText(
            text_font,
            text_size,
            ImVec2(min.x + 10.0f, min.y + (size.y - value_size.y) * 0.5f - 1.0f),
            IM_COL32(235, 235, 235, a),
            value);
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
        const char* arrow = u8"\uf4fa";
        const ImVec2 arrow_measure = icon_font->CalcTextSizeA(arrow_size, 1000.0f, 0.0f, arrow);
        draw_list->AddText(
            icon_font,
            arrow_size,
            ImVec2(max.x - 18.0f - arrow_measure.x * 0.5f, min.y + (size.y - arrow_measure.y) * 0.5f),
            IM_COL32(142, 142, 148, a),
            arrow);
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
        return enabled && ImGui::IsMouseHoveringRect(min, max) && ImGui::IsMouseClicked(ImGuiMouseButton_Left);
    }
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
    inline bool combobox(const char* value, const ImVec2& min, const ImVec2& size, float alpha = 1.0f, bool enabled = true)
    {
        Context& ctx = context();
        return combobox(value, min, size, fallback_font(ctx.text_font), fallback_font(ctx.icon_font), 13.0f, 11.0f, alpha, enabled);
    }
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
    inline bool slider(
        const char* label,
        const ImVec2& child_min,
        float label_y,
        float track_y,
        float& value,
        float& target_value,
        bool& dragging,
        float max_value,
        const char* value_format = "%.0f",
        float alpha = 1.0f,
        bool enabled = true,
        float alpha_multiplier = 1.0f)
    {
        Context& ctx = context();
        ImGuiIO& io = ImGui::GetIO();
        ImDrawList* draw_list = ImGui::GetBackgroundDrawList();
        ImFont* text_font = fallback_font(ctx.text_font);
        const float slider_alpha_f = clamp01(alpha * alpha_multiplier);
        const int slider_alpha = static_cast<int>(255.0f * slider_alpha_f);
        if (slider_alpha <= 0)
            return false;
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
        const ImVec2 track_min(child_min.x + 13.0f, track_y);
        const ImVec2 track_max(child_min.x + 274.0f, track_y + 5.0f);
        const ImVec2 hit_min(track_min.x, track_y - 9.0f);
        const ImVec2 hit_max(track_max.x, track_y + 12.0f);
        const bool hovered =
            io.MousePos.x >= hit_min.x && io.MousePos.x <= hit_max.x &&
            io.MousePos.y >= hit_min.y && io.MousePos.y <= hit_max.y;
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
        bool changed = false;
        if (enabled && slider_alpha_f > 0.4f && hovered && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
            dragging = true;
        if (!io.MouseDown[ImGuiMouseButton_Left])
            dragging = false;
        if (enabled && dragging)
        {
            const float width = track_max.x - track_min.x;
            const float t = width > 0.0f ? (io.MousePos.x - track_min.x) / width : 0.0f;
            const float next_target = ImClamp(t, 0.0f, 1.0f) * max_value;
            changed = next_target != target_value;
            target_value = next_target;
        }
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
        const float smooth_step = ctx.delta_time * 18.0f > 1.0f ? 1.0f : ctx.delta_time * 18.0f;
        value += (target_value - value) * smooth_step;
        if (value - target_value < 0.01f && target_value - value < 0.01f)
            value = target_value;
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
        const float value_t = max_value > 0.0f ? ImClamp(value / max_value, 0.0f, 1.0f) : 0.0f;
        const float fill_x = track_min.x + (track_max.x - track_min.x) * value_t;
        char value_text[32];
        ImFormatString(value_text, IM_ARRAYSIZE(value_text), value_format, value);
        const float slider_text_size = 13.0f;
        const ImVec2 value_size = text_font->CalcTextSizeA(slider_text_size, 1000.0f, 0.0f, value_text);
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
        draw_list->AddText(text_font, slider_text_size, ImVec2(child_min.x + 13.0f, label_y), IM_COL32(205, 205, 210, slider_alpha), label);
        draw_list->AddText(text_font, slider_text_size, ImVec2(child_min.x + 274.0f - value_size.x, label_y), IM_COL32(88, 88, 94, slider_alpha), value_text);
        draw_list->AddRectFilled(track_min, track_max, IM_COL32(24, 24, 24, slider_alpha), 2.5f);
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
        if (fill_x > track_min.x + 0.5f)
        {
            const float track_rounding = 2.5f;
            const float track_center_y = (track_min.y + track_max.y) * 0.5f;
            const float fill_end_x = ImClamp(fill_x, track_min.x, track_max.x);
            const bool fill_reaches_end = fill_end_x >= track_max.x - 0.5f;
            const ImU32 fill_start_color = IM_COL32(148, 140, 246, slider_alpha);
            const ImU32 fill_end_color = IM_COL32(129, 122, 222, slider_alpha);
            draw_list->AddCircleFilled(ImVec2(track_min.x + track_rounding, track_center_y), track_rounding, fill_start_color, 24);
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
            const float fill_rect_start_x = track_min.x + track_rounding;
            const float fill_rect_end_x = fill_reaches_end ? track_max.x - track_rounding : fill_end_x;
            if (fill_rect_end_x > fill_rect_start_x)
            {
                draw_list->AddRectFilledMultiColor(
                    ImVec2(fill_rect_start_x, track_min.y),
                    ImVec2(fill_rect_end_x, track_max.y),
                    fill_start_color,
                    fill_end_color,
                    fill_end_color,
                    fill_start_color);
            }
            if (fill_reaches_end)
                draw_list->AddCircleFilled(ImVec2(track_max.x - track_rounding, track_center_y), track_rounding, fill_end_color, 24);
        }
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
        draw_list->AddCircleFilled(ImVec2(fill_x, track_y + 2.5f), 7.0f, IM_COL32(44, 44, 44, slider_alpha), 24);
        return changed;
    }
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
    inline bool checkbox(
        const char* id,
        const char* label,
        const ImVec2& row_min,
        const ImVec2& row_size,
        const ImVec2& label_pos,
        const ImVec2& toggle_min,
        bool& enabled,
        float& toggle_fade,
        float& hover_fade,
        ImFont* font,
        float text_size,
        float delta_time,
        bool foreground = false,
        float alpha = 1.0f,
        bool interactive = true,
        bool hover_managed = false)
    {
        bool hovered = false;
        bool clicked = false;
        if (interactive)
        {
            const ImVec2 row_max(row_min.x + row_size.x, row_min.y + row_size.y);
            hovered = mouse_in_rect(row_min, row_max);
            clicked = hovered && ImGui::IsMouseClicked(ImGuiMouseButton_Left);
            if (clicked)
                enabled = !enabled;
            if (hovered)
                ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
        }
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
        toggle_fade += ((enabled ? 1.0f : 0.0f) - toggle_fade) * (delta_time * 10.0f > 1.0f ? 1.0f : delta_time * 10.0f);
        if (!hover_managed)
            hover_fade += ((hovered ? 1.0f : 0.0f) - hover_fade) * (delta_time * 14.0f > 1.0f ? 1.0f : delta_time * 14.0f);
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
        ImDrawList* draw_list = foreground ? ImGui::GetForegroundDrawList() : ImGui::GetBackgroundDrawList();
        const int alpha_channel = static_cast<int>(255.0f * clamp01(alpha));
        const float enabled_fade = smooth(toggle_fade);
        const float hovered_fade = smooth(hover_fade);
        const int label_r = lerp_channel(62, 255, enabled_fade);
        const int label_g = lerp_channel(62, 255, enabled_fade);
        const int label_b = lerp_channel(66, 255, enabled_fade);
        draw_list->AddText(
            font,
            text_size,
            label_pos,
            IM_COL32(label_r, label_g, label_b, alpha_channel),
            label);
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
        const ImVec2 toggle_size(25.0f, 15.0f);
        const ImVec2 toggle_max(toggle_min.x + toggle_size.x, toggle_min.y + toggle_size.y);
        const int track_r = lerp_channel(32, 148, enabled_fade);
        const int track_g = lerp_channel(32, 140, enabled_fade);
        const int track_b = lerp_channel(34, 246, enabled_fade);
        const int knob_value = lerp_channel(86, 10, enabled_fade);
        const float knob_x = toggle_min.x + 7.5f + (toggle_size.x - 15.0f) * enabled_fade;
        const float knob_radius = 5.5f + (4.6f - 5.5f) * hovered_fade;
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
        draw_list->AddRectFilled(toggle_min, toggle_max, IM_COL32(track_r, track_g, track_b, alpha_channel), toggle_size.y * 0.5f);
        draw_list->AddCircleFilled(
            ImVec2(knob_x, toggle_min.y + toggle_size.y * 0.5f),
            knob_radius,
            IM_COL32(knob_value, knob_value, knob_value, alpha_channel),
            24);
        return clicked;
    }
// made by hasbi, naszynik discord: https://discord.gg/4cmJpPw5Mf - love destroying unknowncheats devs
    inline bool checkbox(
        const char* id,
        const char* label,
        const ImVec2& row_min,
        const ImVec2& row_size,
        const ImVec2& label_pos,
        const ImVec2& toggle_min,
        bool& enabled,
        float& toggle_fade,
        float& hover_fade,
        float text_size = 13.0f,
        bool foreground = false,
        float alpha = 1.0f,
        bool interactive = true,
        bool hover_managed = false)
    {
        Context& ctx = context();
        return checkbox(
            id,
            label,
            row_min,
            row_size,
            label_pos,
            toggle_min,
            enabled,
            toggle_fade,
            hover_fade,
            fallback_font(ctx.text_font),
            text_size,
            ctx.delta_time,
            foreground,
            alpha,
            interactive,
            hover_managed);
    }
}
