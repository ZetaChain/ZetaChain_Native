echo Installing Linux Dependancies...

# Setup Directory

sudo rm -rf linux-deps
mkdir linux-deps

sudo apt-get update
sudo apt-get upgrade

# Setting Up Python

sudo apt-get install build-essential checkinstall
sudo apt-get install libreadline-gplv2-dev libncursesw5-dev libssl-dev libsqlite3-dev tk-dev libgdbm-dev libc6-dev libbz2-dev

version=2.7.14
cd linux-deps
wget https://www.python.org/ftp/python/$version/Python-$version.tgz

tar -xvf Python-$version.tgz
cd Python-$version

./configure
make -j6
sudo make install -j6

cd ../..

# Setting Up JDK

sudo apt-get install openjdk-8-jdk
apt-cache search jdk
export JAVA_HOME=/usr/lib/jvm/java-8-openjdk-amd64
export PATH=$PATH:/usr/lib/jvm/java-8-openjdk-amd64/bin
javac --version

# Setting Up Gradle

sudo apt-get install gradle
gradle -v

# Setup OpenCL

sudo apt install ocl-icd-opencl-dev