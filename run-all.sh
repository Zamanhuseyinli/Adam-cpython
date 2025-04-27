
set -e  # Hata olursa scripti durdur
set -o pipefail  # Pipe içindeki hatayı da yakala

# Python interpreter listesi
python_versions=(
"/opt/python/cp310-cp310/bin/python"
"/opt/python/cp311-cp311/bin/python"
"/opt/python/cp312-cp312/bin/python"
"/opt/python/cp313-cp313/bin/python"
"/opt/python/cp37-cp37m/bin/python"
"/opt/python/cp39-cp39/bin/python"
"/opt/python/cp36-cp36m/bin/python"
"/opt/python/cp38-cp38/bin/python"
)

# Her Python için build işlemi
for python in "${python_versions[@]}"; do
echo "===> Building with: $python"

# Eksik setuptools veya wheel varsa kur
"$python" -m pip install --upgrade pip setuptools wheel auditwheel

# Build işlemi
"$python" setup.py sdist bdist_wheel
done

# Wheel'leri onarma işlemi
mkdir -p wheelhouse/

for whl in dist/*.whl; do
echo "===> Repairing: $whl"
auditwheel repair "$whl" --wheel-dir wheelhouse/
done

echo "===> Tüm işlemler tamamlandı. Wheel dosyaları 'wheelhouse/' klasöründe."



