from setuptools import setup, find_packages

setup(
    name='servo-library',
    version='0.1.0',
    author='Your Name',
    author_email='your.email@example.com',
    description='A library for controlling servo motors on 328P and Raspberry Pi platforms.',
    packages=find_packages(where='src'),
    package_dir={'': 'src'},
    install_requires=[
        # List any dependencies here, e.g., 'numpy', 'pytest', etc.
    ],
    classifiers=[
        'Programming Language :: Python :: 3',
        'License :: OSI Approved :: MIT License',
        'Operating System :: OS Independent',
    ],
    python_requires='>=3.6',
)