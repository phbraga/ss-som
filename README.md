# SS-SOM
The model Semi-Supervised Self-Organizing Map (SS-SOM), published in the World Conference on Computational Intelligence (WCCI), precisely at the International Joint Conference on Neural Networks (IJCNN) in 2018. It is a small part of the [Prototype Based Machine Learning (PBML)](https://github.com/hfbassani/pbml/) project. The SS-SOM is also available [there](https://github.com/hfbassani/pbml/blob/master/phmb4/Models).

### Requirements:

1. You must have a file containing all the paths to the datasets you want to use. You can follow [this](https://github.com/hfbassani/pbml/blob/master/phmb4/Parameters/inputPathsTrain) example.

2. You must have a parameters file:

   To run SS-SOM, there is 11 parameters to set:

   1. a_t
   2. lp
   3. dsbeta
   4. age_wins
   5. e_b
   6. e_n
   7. epsilon_ds
   8. minwd
   9. epochs
   10. seed
   11. push_rate

   You can follow [this](https://github.com/hfbassani/pbml/blob/master/phmb4/Parameters/SSSOM_0) example, where the first eleven rows represent the first set of parameters, the next 11 rows the second set and so on.

### Parameters Generation

See [params-gen](https://github.com/hfbassani/pbml/tree/master/params-gen/)

### Running:

1. Make sure you fill the requirements.

2. Open the NetbeansProject with Netbeans

3. Set the main arguments for the program:

  ```
  -i: this flag is used to get the path to the file containing all the paths to the datasets to be used.

  -r: this flag is used to get the path of the folder with the test files

  -r: this flag is used to get the path to the results folder

  -p: this flag is used to get the path to the parameters file

  -s[optional]: this flag disables the subspace clustering mode. With this flag, each sample will be assigned to a single cluster.

  -s[optional]: this flag is used to define if the training and test experiments will be run.

  -f[optional]: this flag disables the noisy filtering and all samples will be assigned to a cluster.

  -n[optional]: this flag is used to define if the data needs to be normalized.

  ...and more.
  ```
  For example, to run experiments for [these](https://github.com/hfbassani/pbml/tree/master/Datasets/Realdata) real datasets, you must use -s and -f flags, so that the arguments will be as follows:

  ```
  -i ../../Parameters/inputPathsTrain01 -t ../../Parameters/inputPathsTest -r pray5-l01/ -p ../../Parameters/pray5_0 -s -c
  ```

  After that, you can run your metrics based on the results file.

### Metrics Calculation

See [py-scripts](https://github.com/hfbassani/pbml/tree/master/phmb4/py_scripts).
