FROM alpine:3.19

LABEL version="2.5.0" \
      author="Author Paul Sec, Sullo and pxcs (https://github.com/PaulSec), valdenikto User https://github.com/drwetter" \
      docker_build="docker build -t pxcs/valdenikto:2.5.0 ." \
      docker_run_basic="docker run --rm pxcs/valdenikto:2.5.0 -h http://www.example.com" \
      docker_run_advanced="docker run --rm -v $(pwd):/tmp pxcs/valdenikto:2.5.0 -h http://www.example.com -o /tmp/out.json"

RUN echo 'Installing packages for valdenikto.'
RUN apk add --update --no-cache --virtual .build-deps \
     perl \
     perl-net-ssleay

RUN echo 'Creating the valdenikto group.' \
  && addgroup valdenikto \
  && echo 'Creating the valdenikto user.' \
  && adduser -G valdenikto -g "valdenikto user" -s /bin/sh -HD valdenikto

ENV  PATH=${PATH}:/opt/valdenikto
USER valdenikto

COPY --chown=valdenikto:valdenikto ["program/", "/opt/valdenikto"]
ENTRYPOINT ["valdenikto.pl"]
