/*
 * SPDX-License-Identifier: MIT
 *
 * Copyright 2023 Jérémie Galarneau <jeremie.galarneau@gmail.com>
 */

#include "board.hpp"
#include "display/renderer.hpp"
#include "globals.hpp"

namespace nd = nsec::display;
namespace ns = nsec::scheduling;

namespace {
void show_frametime(Adafruit_SSD1306& canvas, nsec::scheduling::relative_time_ms frame_time)
{
	canvas.setCursor(0, 0);
	canvas.print("frame time: ");
	canvas.print(frame_time, 10);
	canvas.printf("ms");
}
} // namespace

nd::renderer::renderer(nd::screen **focused_screen) noexcept :
	periodic_task(nsec::config::display::refresh_period_ms),
	_display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET),
	_focused_screen{ focused_screen }
{
	nsec::g::the_scheduler.schedule_task(*this);
}

void nd::renderer::setup() noexcept
{
	_display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS);
	_display.setTextColor(SSD1306_WHITE);
	_display.clearDisplay();
	_display.setTextSize(1);
	_display.display();
}

void nd::renderer::run(scheduling::absolute_time_ms current_time_ms) noexcept
{
	_display.clearDisplay();
	const auto entry = millis();
	focused_screen().render(current_time_ms, _display);
	const auto exit = millis();
	_display.display();

	_last_frame_time_ms = exit - entry;
}