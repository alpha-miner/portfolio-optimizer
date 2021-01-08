import io
from setuptools import setup
from setuptools import find_packages

setup(
    name='portfolio-optimizer',
    version='0.1.0',
    packages=find_packages(),
    install_requires=io.open('requirements.txt', encoding='utf8').read(),
    url='',
    license='',
    author='',
    author_email='wegamekinglc@hotmail.com',
    description=''
)
