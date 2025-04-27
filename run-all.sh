
set -e 
set -o pipefail  

#
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


for python in "${python_versions[@]}"; do
echo "===> Building with: $python"


"$python" -m pip install --upgrade pip setuptools wheel auditwheel

"$python" setup.py sdist bdist_wheel
done


mkdir -p wheelhouse/

for whl in dist/*.whl; do
echo "===> Repairing: $whl"
auditwheel repair "$whl" --wheel-dir wheelhouse/
done

echo "===> Tüm işlemler tamamlandı. Wheel dosyaları 'wheelhouse/' klasöründe."



