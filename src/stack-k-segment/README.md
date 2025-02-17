# Data structure description

The k-segment stack is a relaxed stack implemented as a stack of array segments of size k. Theses segments are operated on in lifo order, but without internal ordering within the segments (leading to k out-of-order relaxation).

Note: This design is not that well engineered, and relies on global variables such as the segment size and thread_id. Keep this in mind if you want to extend the design with more tests.

## Origin

The design is from the k-stack from the paper [Quantitative relaxation of concurrent data structures](https://doi.org/10.1145/2429069.2429109), and the implementation is from the evaluation of the [first 2D paper](https://doi.org/10.4230/LIPIcs.DISC.2019.31).

## Main Author

Adones Rukundo
