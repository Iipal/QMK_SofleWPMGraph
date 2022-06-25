# Major changes:

```
rules.mk:
/**
 * No more RGB lights suppports because with WPM_ENABLE.
 * There is no enough memory on chip to enable both of this options.
 **/
RGBLIGHT_ENABLE = no
WPM_ENABLE = yes

config.h:
# define OLED_TIMEOUT 20000 // now OLED screens offs after 20 seconds.
# define OLED_BRIGHTNESS 50 // Max: 255, about 19% brightness
# define SPLIT_WPM_ENABLE   // Enable WPM across split keyboards (+268).
# define SPLIT_OLED_ENABLE  // Sync on/off OLED state between halves (+100).
```

### Left side texts:

```
Qwrt -> QWRT
Clmk -> GAME
Mod -> LYR

Base -> *unchanged*
Raise -> RSE
Lower -> LWR

CPSLK -> *removed*
```

### Right side:

```
WPM and typing graph.
```

### Compile:

`qmk compile -kb sofle -km via`

### Authority

The graph code originally taken from [this Reddit post](https://www.reddit.com/r/nullbits/comments/iizlh2/wpm_graph_for_all_your_oled_screens/) with minor adaptation for Sofle v1\2.

<br />

### My Layout for VIA and Compiled HEX

[Layout](sofle_layout.json)

[Compiled HEX](compiled/sofle_rev1_via.hex)

<br /><br /><br />

# Sofle Keyboard

![SofleKeyboard version 1](https://raw.githubusercontent.com/josefadamcik/SofleKeyboard/master/Images/IMG_20200126_114622.jpg)

Sofle is 6×4+5 keys column-staggered split keyboard. Based on Lily58, Corne and Helix keyboards.

More details about the keyboard on my blog: [Let me introduce you SofleKeyboard - a split keyboard based on Lily58 and Crkbd](https://josef-adamcik.cz/electronics/let-me-introduce-you-sofle-keyboard-split-keyboard-based-on-lily58.html)

The current (temporary) build guide and a build log is available here: [SofleKeyboard build log/guide](https://josef-adamcik.cz/electronics/soflekeyboard-build-log-and-build-guide.html)

- Keyboard Maintainer: [Josef Adamcik](https://josef-adamcik.cz) [Twitter:@josefadamcik](https://twitter.com/josefadamcik)
- Hardware Supported: SofleKeyboard PCB, ProMicro
- Hardware Availability: [PCB & Case Data](https://github.com/josefadamcik/SofleKeyboard)

Make example for this keyboard (after setting up your build environment):

    make sofle:default

Flash the default keymap:

    make sofle:default:avrdude

Press reset button on he keyboard when asked.

Disconnect the first half, connect the second one and repeat the process.

See the [build environment setup](https://docs.qmk.fm/#/getting_started_build_tools) and the [make instructions](https://docs.qmk.fm/#/getting_started_make_guide) for more information. Brand new to QMK? Start with our [Complete Newbs Guide](https://docs.qmk.fm/#/newbs).
