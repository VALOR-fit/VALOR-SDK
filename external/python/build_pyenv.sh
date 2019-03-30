if [[ $(which pip) != "/cvmfs/"* ]] ; then
  wget https://bootstrap.pypa.io/get-pip.py
  python get-pip.py --prefix "${PWD}"
  piplibdir="$(find lib -name 'site-packages')"

  export PYTHONPATH="${piplibdir}:${PYTHONPATH}"

  ./bin/pip install virtualenv --prefix "${PWD}"
  ./bin/virtualenv valor

  rm -r ./bin ./lib get-pip.py

else
  venv_ver=16.4.3
  venv_tarball=virtualenv-${venv_ver}.tar.gz
  curl --location --output ${venv_tarball} https://github.com/pypa/virtualenv/tarball/${venv_ver}
  tar xf ${venv_tarball}
  venv_dir=$(ls -d pypa-virtualenv-* | head -n 1)
  ${venv_dir}/virtualenv.py valor
  rm -r ${venv_dir}
  rm ${venv_tarball}
fi

. valor/bin/activate

pip install virtualenv pyreadline ipython numpy

echo -e "\n\n"
echo "Built VALOR python dependencies. Stuff will probably not work unless you start a new terminal, so do that."

