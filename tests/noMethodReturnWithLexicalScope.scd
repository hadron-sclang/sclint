// True Positive:
Function {
    prTry {
        var result, thread = thisThread;
        var next = thread.exceptionHandler,
        wasInProtectedFunc = Exception.inProtectedFunction;
        thread.exceptionHandler = {|error|
            thread.exceptionHandler = next; // pop
            ^error  //+//: 1,13
        };
        Exception.inProtectedFunction = true;
        result = this.value;
        Exception.inProtectedFunction = wasInProtectedFunc;
        thread.exceptionHandler = next; // pop
        ^result
    }
}

// False Positive:
Quark {
    dependencies {
        var deps = this.data['dependencies'] ?? {^[]};  // should not error
        if(deps.isSequenceableCollection.not, {
            ("Invalid dependencies " + this + deps).warn;
            ^[]
        });
        ^deps.collect({ |dep|
            var q = Quark.parseDependency(dep, this);
            if(q.isNil, {
                "% not found".format(dep).warn;
            });
            q
        }).select({ |it| it.notNil });
    }
}