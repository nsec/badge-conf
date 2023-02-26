/*
 * SPDX-License-Identifier: MIT
 *
 * Copyright 2023 Jérémie Galarneau <jeremie.galarneau@gmail.com>
 */

#include "board.hpp"
#include "display/renderer.hpp"
#include "globals.hpp"

namespace nd = nsec::display;

nd::renderer::renderer() noexcept :
	periodic_task(nsec::config::display::refresh_period_ms),
	_display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET)
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
	const auto entry = millis();
	_display.clearDisplay();
	_display.setCursor(0, 0);
	_display.print("frame time: ");
	_display.print(_last_frame_time_ms, 10);
	_display.printf("ms");
	_display.display();
	const auto exit = millis();
	_last_frame_time_ms = exit - entry;
}