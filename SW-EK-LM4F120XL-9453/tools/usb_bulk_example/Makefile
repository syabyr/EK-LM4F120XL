all:
	gcc -o usb_bulk_example usb_bulk_example.c `pkg-config --cflags --libs libusb-1.0`
	gcc -o usb_bulk_loop usb_bulk_loop.c `pkg-config --cflags --libs libusb-1.0`
clean:
	rm -fr usb_bulk_example usb_bulk_loop