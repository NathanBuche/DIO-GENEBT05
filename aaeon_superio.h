int aaeon_display_gpio_mode(int pin);
int aaeon_display_gpio_value(int pin);
void aaeon_set_gpio_value(int pin, int value);
void aaeon_set_gpio_mode(int pin, int mode);
int aaeon_open_superio();
int aaeon_close_superio();
void aaeon_set_dio_base(int diobase);
int aaeon_get_superio_chip();
int aaeon_get_current_dio_base();


enum superiochips{chip_f81866d};

