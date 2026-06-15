############################################
# Copyright (c) 2026 Shun/修海 (@shun4midx) #
# Project: Shun4miTeX                      #
# File Type: Docker file                   #
# File: Dockerfile                         #
############################################

FROM debian:bookworm-slim

RUN apt-get update && apt-get install -y \
    texlive-latex-base \
    texlive-latex-extra \
    texlive-fonts-recommended \
    poppler-utils \
    imagemagick \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /work

COPY render.sh /usr/local/bin/render.sh
RUN chmod +x /usr/local/bin/render.sh

ENTRYPOINT ["/usr/local/bin/render.sh"]