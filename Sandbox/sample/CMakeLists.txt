if (TARGET tinyusb_device)
	add_executable(sample
	sample.c
			)

	# pull in common dependencies
	target_link_libraries(sample pico_stdlib pico_time hardware_gpio)

	# create map/bin/hex file etc.
	pico_add_extra_outputs(sample)

	# add url via pico_set_program_url
	example_auto_set_url(sample)

	# enable usb output, disable uart output
	pico_enable_stdio_usb(sample 1)
	pico_enable_stdio_uart(sample 0)

elseif(PICO_ON_DEVICE)
	message(WARNING "not building hello_usb because TinyUSB submodule is not initialized in the SDK")
endif()
