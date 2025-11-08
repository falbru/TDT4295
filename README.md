# TDT4295

## Testing

We use Unity as our testing framework. To run the tests, ensure that the project is built with `BUILD_TESTS` set to `ON`. Then use `ctest` to run all the tests:

```sh
ctest --test-dir build --output-on-failure
```
