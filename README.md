# SS-SOM

A Semi-Supervised Self-Organizing Map for Clustering and Classification

Sample code to the model proposed in https://ieeexplore.ieee.org/abstract/document/8489675

It was presented in the 2018 World Conference on Computational Intelligence (WCCI), precisely at the International Joint Conference on Neural Networks (IJCNN), in Rio de Janeiro, Brazil.

### Cite:

Please cite our paper if you use this code in your own work:

```bibtex
@inproceedings{braga2018semi,
  title         = {A Semi-Supervised Self-Organizing Map for Clustering and Classification},
  author        = {Braga, Pedro HM and Bassani, Hansenclever F},
  booktitle     = {2018 International Joint Conference on Neural Networks (IJCNN)},
  pages         = {1--8},
  year          = {2018},
  organization  = {IEEE}
}
```

### Requirements:

1. You must have a file containing all the paths to the datasets you want to use. You can follow [this](https://github.com/hfbassani/pbml/blob/master/phmb4/Parameters/inputPathsTrain) example.

2. You must have a parameters file:

  To run SS-SOM, there are 11 parameters to set:

   - a_t
   - lp
   - dsbeta
   - age_wins
   - e_b
   - e_n
   - epsilon_ds
   - minwd
   - epochs
   - seed
   - push_rate<br/><br/>

   You can follow [this](https://github.com/hfbassani/pbml/blob/master/phmb4/Parameters/sssom_0) example, where the first eleven rows represent the first set of parameters, the next 11 rows the second set and so on.

   Also, it is important to update the constant _noClass_, if necessary. The default value is 999.

### Parameters Generation

The sample code to generate the parameters with LHS, as in the paper, is available [here](https://github.com/hfbassani/pbml/tree/master/params-gen/).

### Running:

1. Make sure you fill the requirements.

2. Open the NetbeansProject with Netbeans

3. Set the main arguments for the program:

  ```
  -i: this flag is used to get the path to the file containing all the paths to the datasets to be used.

  -t: this flag is used to get the path of the folder with the test files

  -r: this flag is used to get the path to the results folder

  -p: this flag is used to get the path to the parameters file

  -f[optional]: this flag disables the noisy filtering and all samples will be assigned to a cluster.

  -n[optional]: this flag is used to define if the data needs to be normalized.

  -c[optional]: this flag is used to execute a complete train and test experiment.

  ...and more.
  ```
  For example, to run experiments for [these](https://github.com/hfbassani/pbml/tree/master/Datasets/Realdata) real datasets the arguments will be as follows:

  ```
  -i ../../Parameters/inputPathsTrain01 -t ../../Parameters/inputPathsTest -r sssom-l01/ -p ../../Parameters/sssom_0 -c
  ```

  After that, you can evaluate the results folder files.

### Metrics Calculation

See [py-scripts](https://github.com/hfbassani/pbml/tree/master/phmb4/py_scripts).
