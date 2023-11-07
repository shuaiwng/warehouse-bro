if (TARGET tinyusb_device)
	add_executable(firmware
	firmware.c
			)

	# pull in common dependencies
	target_link_libraries(firmware pico_stdlib pico_time hardware_gpio)

	# create map/bin/hex file etc.
	pico_add_extra_outputs(firmware)

	# add url via pico_set_program_url
	example_auto_set_url(firmware)

	# enable usb output, disable uart output
	pico_enable_stdio_usb(firmware 1)
	pico_enable_stdio_uart(firmware 0)

elseif(PICO_ON_DEVICE)
	message(WARNING "not building firmware because TinyUSB submodule is not initialized in the SDK")
endif()
