FROM gcc:12 as build

# Install cmake and ninja
RUN export DEBIAN_FRONTEND=noninteractive \
    && apt-get update \
    && apt-get install -y --no-install-recommends \
    cmake \
    git \
    ninja-build \
    zip \
    && apt-get autoremove -y \
    && apt-get clean -y \
    && rm -rf /var/lib/apt/lists/*

# Add non-root user
RUN useradd -ms /bin/bash rfsc

# Switch to non-root user
USER rfsc

# Create vcpkg source directory
WORKDIR /opt/vcpkg
RUN git clone --depth=1 https://github.com/Microsoft/vcpkg.git .

# Install vcpkg
RUN ./bootstrap-vcpkg.sh

# Configure vcpkg
ENV VCPKG_ROOT=/opt/vcpkg \
    VCPKG_DOWNLOADS=/opt/vcpkg/vcpkg-downloads

ENV PATH="${PATH}:${VCPKG_ROOT}"

# Create source directory
WORKDIR /usr/src/echo_server
RUN chown rfsc:rfsc /usr/src/echo_server

# Install dependencies
RUN mkdir build/
COPY ./vcpkg.json .
RUN vcpkg install --x-install-root build/vcpkg_installed

# Copy source
COPY . /usr/src/echo_server

# Build echo_server
RUN cmake -B build/ --preset release
RUN cmake --build build/

# Run tests
RUN ctest --preset tests -R business_rules
RUN ctest --preset tests -R concurrency
RUN ctest --preset tests -R cipher

FROM gcc:12

LABEL org.opencontainers.image.source=https://github.com/aruraune/echo_server

WORKDIR /opt/echo_server

COPY --from=build /usr/src/echo_server/build/server/echo_server .

# Add non-root user
RUN useradd -ms /bin/bash rfsc

# Switch to non-root user
USER rfsc

ENTRYPOINT [ "./echo_server" ]
