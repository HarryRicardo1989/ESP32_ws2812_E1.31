# ESP32_ws2812_E1.31

Codigo fonte utilizado nos ESP32 do Show de Natal Paschoali em Ribeirao Preto SP

[Show de leds de natal Familia Paschoali 2019 ( Ribeirão Preto-SP-BR)](https://www.youtube.com/watch?v=k5JrR18vDP0&t=6s)

## ESP32+Ws2812b Config

On UDPTask.h you need to config the number of leds and the number of universes from e1.31 transmiter.

```C++
#define NUM_LEDS  <your number of leds>      
#define CHANNEL_COUNT (NUM_LEDS*3)  // (quantity of channels)
#define UNIVERSE_COUNT <number of Universes>
#define LEDS_PER_UNIVERSE (NUM_LEDS/UNIVERSE_COUNT) // per universe
```

## Contributing
Pull requests are welcome. For major changes, please open an issue first to discuss what you would like to change.

Please make sure to update tests as appropriate.

## License
[MIT](https://choosealicense.com/licenses/mit/)