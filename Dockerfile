FROM victorhcm/opencv:3.2.0-python2.7

COPY . /app

RUN mkdir app/build
WORKDIR /app/build

RUN cmake -D CMAKE_BUILD_TYPE=Release -D CMAKE_INSTALL_PREFIX=/usr/local ..
RUN make VERBOSE=1
RUN make install

ENTRYPOINT ["/usr/local/bin/camodet"]
CMD ["--help"]
