
def new_line(fd):
    fd.write("\n")

def arg_struct(f, Method,DataTypes):
    if len(Method.in_args) > 0:
        f.write(f"                    struct {Method.name}Input")
        new_line(f)
        f.write("                    {")
        new_line(f)
        for in_arg in Method.in_args:
            Pathlist = in_arg.path.split("/")
            Type = DataTypes[Pathlist[-2]][Pathlist[-1]]
            f.write(f"                        {Type.Instantation(in_arg.name)}")
        f.write("                    private:")
        new_line(f)
        f.write("                        template <typename Archive>")
        new_line(f)
        f.write("                    void serialize(Archive &ar, const unsigned int version)")
        new_line(f)
        f.write("                        {")
        new_line(f)
        for in_arg in Method.in_args:
            f.write(f"                        ar &{in_arg.name};")
            new_line(f)
        f.write("                        }")
        new_line(f)
        f.write("                        friend class boost::serialization::access;")
        new_line(f)
        f.write("                    };")
        new_line(f)
        new_line(f)

    if len(Method.out_args) > 0:
        f.write(f"                    struct {Method.name}Output")
        new_line(f)
        f.write("                    {")
        new_line(f)
        for out_arg in Method.out_args:
            Pathlist = out_arg.path.split("/")
            Type = DataTypes[Pathlist[-2]][Pathlist[-1]]
            f.write(f"                        {Type.Instantation(out_arg.name)}")
            new_line(f)
        f.write("                    private:")
        new_line(f)
        f.write("                        template <typename Archive>")
        new_line(f)
        f.write("                    void serialize(Archive &ar, const unsigned int version)")
        new_line(f)
        f.write("                        {")
        new_line(f)
        for out_arg in Method.out_args:
            f.write(f"                        ar &{out_arg.name};")
            new_line(f)
        f.write("                        }")
        new_line(f)
        f.write("                        friend class boost::serialization::access;")
        new_line(f)
        f.write("                    };")
        new_line(f)
        new_line(f)

def field_struct(f, Field, DataTypes):
    f.write(f"                    struct {Field.name}Field")
    new_line(f)
    f.write("                    {")
    new_line(f)

    Pathlist = Field.path.split("/")
    Type = DataTypes[Pathlist[-2]][Pathlist[-1]]
    f.write(f"                        {Type.Instantation(Field.name)}")

    f.write("                    private:")
    new_line(f)
    f.write("                        template <typename Archive>")
    new_line(f)
    f.write("                    void serialize(Archive &ar, const unsigned int version)")
    new_line(f)
    f.write("                        {")
    new_line(f)

    f.write(f"                        ar &{Field.name};")
    new_line(f)
    
    f.write("                        }")
    new_line(f)
    f.write("                        friend class boost::serialization::access;")
    new_line(f)
    f.write("                    };")
    new_line(f)
    new_line(f)


def Datatypeslisting(shared_type, list, DataTypes):
    for k in DataTypes["shared_types"][shared_type].subTypes:
        Datatypeslisting(k, list, DataTypes)
        list.append(k) 

def unique(sequence):
    seen = set()
    return [x for x in sequence if not (x in seen or seen.add(x))]