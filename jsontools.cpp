#include "include/jsontools.h"

void json_parse(json_object * jobj)
{
    enum json_type type;
    json_object_object_foreach(jobj, key, val)
    {
        type = json_object_get_type(val);
        switch (type)
        {
//            case json_type_string: printf("type: json_type_string, ");
//            printf("value: %sn", json_object_get_string(val));
            break;
        }
    }
}
