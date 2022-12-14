// True Positive:
Function {
    prTry {
        var result, thread = thisThread;
        var next = thread.exceptionHandler;
        thread.exceptionHandler = {|error|
            thread.exceptionHandler = next; // pop
            ^error  //+//: avoidMethodReturnInNamedFunctions,12
        };
        result = this.value;
        thread.exceptionHandler = next; // pop
        ^result
    }
}
