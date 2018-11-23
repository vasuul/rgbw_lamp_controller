PROJ := controller

${PROJ}: main.cc spi.cc spi.hh rgbw_strip.hh
	g++ -Wall -O2 -std=c++11 main.cc spi.cc -o controller

install:
	systemctl stop lamp
	cp ${PROJ} /usr/local/bin

start:
	systemctl start lamp

clean:
	rm -r -f ${PROJ}

