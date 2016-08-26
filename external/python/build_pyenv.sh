wget https://bootstrap.pypa.io/get-pip.py

python get-pip.py --prefix "${PWD}"

piplibdir="$(find lib -name 'site-packages')"

export PYTHONPATH="${piplibdir}:${PYTHONPATH}"

./bin/pip install virtualenv --prefix "${PWD}"
./bin/virtualenv valor

rm -r ./bin ./lib get-pip.py

. valor/bin/activate

pip install pyreadline ipython numpy


