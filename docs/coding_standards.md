# Coding Standards

In an effort to keep files readable, reduce bugs and make onboarding of new maintainers easier, the project follows a few coding standards.

## Unit Tests

If a function can be tested in some reasonable fashion, it should be. Please don't test your code ad-hoc when its first written, then never do so again. Instead, include unit tests via the gtest framework. This way,  the tests can be varified after each change, which will reduce the frequency of regressions in the library.

## Coding Style

For consistency (and also because it looks good), we follow Google's C++ style guide, located [here](https://google.github.io/styleguide/cppguide.html). Don't feel like you need to read the entire thing at once, but do your best, especially for things like file naming, class naming, etc. Possibly the easiest way to "get it" is to look at the other files in the jetreader repo, and copy their style :)