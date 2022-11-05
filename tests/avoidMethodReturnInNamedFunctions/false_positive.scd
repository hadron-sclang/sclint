// False Positive:
Quark {
    dependencies {
        var deps = this.data['dependencies'] ?? {^[]};  // should not error
        if(deps.isSequenceableCollection.not, {
            ("Invalid dependencies " + this + deps).warn;
            ^[]
        });
    }
}
