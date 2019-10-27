#!/bin/bash

home_path=${PWD%et*}

export SDK_PATH=${home_path}
export BIN_PATH=${home_path}"bin"
#!/bin/bash

echo "gen_misc.sh version 20150511"
echo ""

    boot=new
    app=1
    spi_speed=40
    spi_mode=QIO
    spi_size_map=4
    
touch user/user_main.c

echo ""
echo "start..."
echo ""

make COMPILE=gcc BOOT=$boot APP=$app SPI_SPEED=$spi_speed SPI_MODE=$spi_mode SPI_SIZE_MAP=$spi_size_map
