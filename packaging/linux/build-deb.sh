#!/usr/bin/env bash
set -euo pipefail

VERSION="${1:?Usage: build-deb.sh VERSION BINARY_PATH}"
BINARY="${2:?Usage: build-deb.sh VERSION BINARY_PATH}"
SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
REPO_ROOT="$(cd "$SCRIPT_DIR/../.." && pwd)"

PKG="examwebbrowser_${VERSION}_amd64"
ROOT="$REPO_ROOT/$PKG"

rm -rf "$ROOT"
mkdir -p "$ROOT/DEBIAN"
mkdir -p "$ROOT/opt/examwebbrowser/images"
mkdir -p "$ROOT/usr/bin"
mkdir -p "$ROOT/usr/share/applications"

# Control file
cat > "$ROOT/DEBIAN/control" <<EOF
Package: examwebbrowser
Version: $VERSION
Section: education
Priority: optional
Architecture: amd64
Depends: libqt6webenginewidgets6 (>= 6.2), libqt6widgets6 (>= 6.2)
Maintainer: ExamWebBrowser <contact@examwebbrowser.org>
Description: Web browser for exam environments
 A locked-down web browser designed for secure online examinations.
EOF

# Binary
install -m 755 "$BINARY" "$ROOT/opt/examwebbrowser/ExamWebBrowser"

# Images (only the ones used at runtime)
cp "$REPO_ROOT/images/ExamWebBrowser.png" "$ROOT/opt/examwebbrowser/images/"
cp "$REPO_ROOT/images/home.svg"           "$ROOT/opt/examwebbrowser/images/"
cp "$REPO_ROOT/images/logo.svg"           "$ROOT/opt/examwebbrowser/images/"

# Symlink in PATH
ln -s /opt/examwebbrowser/ExamWebBrowser "$ROOT/usr/bin/examwebbrowser"

# Desktop file
cp "$SCRIPT_DIR/examwebbrowser.desktop" "$ROOT/usr/share/applications/"

# Build the .deb
dpkg-deb --build --root-owner-group "$ROOT"
mv "$PKG.deb" "$REPO_ROOT/"

echo "Built: $REPO_ROOT/$PKG.deb"
