pkgname='desktopper'
pkgver=1
pkgrel=1
pkgdesc="Application to create desktop entry files"
arch=('x86_64')
url="https://github.com/tonilipponen/desktopper"
license=('GPL')
depends=('glibc' 'gtk4' 'libadwaita')
makedepends=('git' 'cmake' 'gcc')
source=()

pkgver() {
  printf "r%s.%s" "$(git rev-list --count HEAD)" "$(git rev-parse --short=7 HEAD)"
}

build() {
    cd $startdir
    cmake -B build
    cmake --build build --parallel
}

package() {
    cd $startdir
    install -Dm755 ./build/desktopper "$pkgdir/usr/bin/desktopper"
}
