evolve_l
===

Lindenmayer system variant that has forward mapping instead of single character

Genes are represented as rules, a from -> to pair. 

Bots evolve concurrently with asexual and sexual reproduction. The best/alpha female asexually reproduces, the best half use sexual reproduction with each other, and the worse half of the generation dies. 

Mutation happens on a micro scale and macro scale, through individual rule modifications as well as new rules completely.

Somewhat interesting as this approach is able to evolve rewrite systems that are more than a handful of iterations deep relatively quickly.

Our goal is to generate a series of rules that over N iterations will transform INPUT to TARGET

The scoring function for this is leichtenshtein distance.