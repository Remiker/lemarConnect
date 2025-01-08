# syntax=docker/dockerfile:1

# Comments are provided throughout this file to help you get started.
# If you need more help, visit the Dockerfile reference guide at
# https://docs.docker.com/go/dockerfile-reference/

# Want to help us make this template better? Share your feedback here: https://forms.gle/ybq9Krt8jtBL3iCk7

ARG PYTHON_VERSION=3.11.6
FROM python:${PYTHON_VERSION}-slim as base

# Prevents Python from writing pyc files.
ENV PYTHONDONTWRITEBYTECODE=1
## Installing Node.js
ENV NODE_ENV production
ENV NODE_VERSION 20.17.0
# Keeps Python from buffering stdout and stderr to avoid situations where
# the application crashes without emitting any logs due to buffering.
ENV PYTHONUNBUFFERED=1

WORKDIR /app

# Create a non-privileged user that the app will run under.
# See https://docs.docker.com/go/dockerfile-user-best-practices/

# RUN ARCH= && dpkgArch="$(dpkg --print-architecture)" \
#   && case "${dpkgArch##*-}" in \
#     amd64) ARCH='x64';; \
#     ppc64el) ARCH='ppc64le';; \
#     s390x) ARCH='s390x';; \
#     arm64) ARCH='arm64';; \
#     armhf) ARCH='armv7l';; \
#     i386) ARCH='x86';; \
#     *) echo "unsupported architecture"; exit 1 ;; \
#   esac \
#   # use pre-existing gpg directory, see https://github.com/nodejs/docker-node/pull/1895#issuecomment-1550389150
#   && export GNUPGHOME="$(mktemp -d)" \
#   && set -ex \
#   && apt-get update && apt-get install -y --no-install-recommends ca-certificates curl wget gnupg dirmngr xz-utils && rm -rf /var/lib/apt/lists/* \
#   # gpg keys listed at https://github.com/nodejs/node#release-keys
#   && for key in \
#     4ED778F539E3634C779C87C6D7062848A1AB005C \
#     141F07595B7B3FFE74309A937405533BE57C7D57 \
#     74F12602B6F1C4E913FAA37AD3A89613643B6201 \
#     DD792F5973C6DE52C432CBDAC77ABFA00DDBF2B7 \
#     CC68F5A3106FF448322E48ED27F5E38D5B0A215F \
#     8FCCA13FEF1D0C2E91008E09770F7A9A5AE15600 \
#     890C08DB8579162FEE0DF9DB8BEAB4DFCF555EF4 \
#     C82FA3AE1CBEDC6BE46B9360C43CEC45C17AB93C \
#     108F52B48DB57BB0CC439B2997B01419BD92F80A \
#     A363A499291CBBC940DD62E41F10027AF002F8B0 \
#   ; do \
#       gpg --batch --keyserver hkps://keys.openpgp.org --recv-keys "$key" || \
#       gpg --batch --keyserver keyserver.ubuntu.com --recv-keys "$key" ; \
#   done \
#   && curl -fsSLO --compressed "https://nodejs.org/dist/v$NODE_VERSION/node-v$NODE_VERSION-linux-$ARCH.tar.xz" \
#   && curl -fsSLO --compressed "https://nodejs.org/dist/v$NODE_VERSION/SHASUMS256.txt.asc" \
#   && gpg --batch --decrypt --output SHASUMS256.txt SHASUMS256.txt.asc \
#   && gpgconf --kill all \
#   && rm -rf "$GNUPGHOME" \
#   && grep " node-v$NODE_VERSION-linux-$ARCH.tar.xz\$" SHASUMS256.txt | sha256sum -c - \
#   && tar -xJf "node-v$NODE_VERSION-linux-$ARCH.tar.xz" -C /usr/local --strip-components=1 --no-same-owner \
#   && rm "node-v$NODE_VERSION-linux-$ARCH.tar.xz" SHASUMS256.txt.asc SHASUMS256.txt \
#   && ln -s /usr/local/bin/node /usr/local/bin/nodejs \
#   # smoke tests
#   && node --version \
#   && npm --version \
#   && apt-mark auto '.*' > /dev/null \
#   && find /usr/local -type f -executable -exec ldd '{}' ';' \
#   | awk '/=>/ { print $(NF-1) }' \
#   | sort -u \
#   | xargs -r dpkg-query --search \
#   | cut -d: -f1 \
#   | sort -u \
#   | xargs -r apt-mark manual \
#   && apt-get purge -y --auto-remove -o APT::AutoRemove::RecommendsImportant=false

RUN groupadd -r rocketchat \
  && useradd -r -g rocketchat rocketchat \
  && mkdir -p /app/uploads \
  && chown rocketchat:rocketchat /app/uploads

VOLUME /app/uploads

ENV RC_VERSION 7.0.0

ARG UID=10001
RUN adduser \
    --disabled-password \
    --gecos "" \
    --home "/nonexistent" \
    --shell "/sbin/nologin" \
    --no-create-home \
    --uid "${UID}" \
    appuser

COPY requirements.txt .
# Download dependencies as a separate step to take advantage of Docker's caching.
# Leverage a cache mount to /root/.cache/pip to speed up subsequent builds.
# Leverage a bind mount to requirements.txt to avoid having to copy them into
# into this layer.
RUN --mount=type=cache,target=/root/.cache/pip \
    --mount=type=bind,source=requirements.txt,target=requirements.txt \
    python -m pip install -r requirements.txt

# ENV DEPLOY_METHOD=docker-official \
#     MONGO_URL=mongodb://Jad:XkI1fA00Zp1ioyQl@193.54.246.79:27017/<LemarConnect>?ssl=true&authSource=admin \
#     HOME=/tmp \
#     PORT=3000 \
#     ROOT_URL=http://localhost:3000 \
#     Accounts_AvatarStorePath=/app/uploads
# Switch to the non-privileged user to run the application.
USER appuser

# Copy the source code into the container.
COPY . .

# Expose the port that the application listens on.
EXPOSE 8000

# Run the application.
CMD python3 -m uvicorn app:app --host=0.0.0.0 --port=8000
