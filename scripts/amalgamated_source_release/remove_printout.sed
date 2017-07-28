#! /usr/bin/sed -nf

# don't remove completely to avoid "label at end of compound statement: expected statement" 
# when printouts are the only statement in the default: section of switch/case block.
s/^std::cout.*<<.*;/assert(1);/
s/^std::cerr.*<<.*;/assert(1);/
s/^cout.*<<.*;/assert(1);/
s/^cerr.*<<.*;/assert(1);/
s/^LOG(.*<<.*;/assert(1);/
