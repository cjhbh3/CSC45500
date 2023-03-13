Name: CJ Hess
Platform: Windows, Compiled and Tested on Linux
Compile: Nothing in particular - the makefile gave me some trouble originally, but I fixed that.
Run: Run using the following ./execName inputFile
Bugs: None, that I know of.
Soultion Details: This was a really interesting project to work on, just as a beginning thought.
As far as my solution, I used the approach that was shown in class. Essentially,
for each nonterminal provided, I created a function that would call another function or print whatever terminal variable came.
My initial implementation was really convoluted and would short circuit pretty quickly, so I had spent most of a day rewriting every function.
When rewriting, I came to a self-check method of "for each choice that a nonterminal has, that should be how many "return SUCCESS" I should have"
which really helped me double check I was correctly implementing the functions. After that, it was one extra late night
trying to get the code to look "pretty".