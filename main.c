#include <stdio.h>


typedef enum ResultStatus {
    OK,
    ERROR,
} ResultStatus;

typedef struct Result {
    ResultStatus status;
    union {
        int ok;
        const char* error;
    } data;
} Result;


Result Ok(int value){
    Result temp = {.status=OK, .data.ok = value};
    return temp;
}

Result Error(char* error){
    Result temp = {.status=ERROR, .data.error = error};
    return temp;
}

Result and_then(Result result, Result (*func)(int)){
    if (result.status == OK){
        return func(result.data.ok);
    } else {
        return result;
    }
}



void on_ok(Result result){
    printf("Final value is: %d\n", result.data.ok);
}

void on_err(Result result){
    printf("Error in the pipeline: %s\n", result.data.error);
}

void finally(Result result, void (*on_ok)(Result result), void (*on_err)(Result result)){
    if (result.status == OK){
        on_ok(result);
    } else if (result.status == ERROR) {
        on_err(result);
    }
}


Result check_boundary(int value){
    int limit = 50;
    if (value<limit){
        return Ok(value);
    }
    static char error_message[250];
    snprintf(error_message, sizeof error_message, "Value is too big %d %d.\n", value, limit);
    return Error(error_message);
}

Result add_one(int x){
    Result p = Ok(x+1);
    return p;
}

int main(){
    int x = 9;
    Result r = Ok(x);
    Result rr = and_then(r, add_one);
    rr = and_then(rr, add_one);
    rr = and_then(rr, add_one);
    rr = and_then(rr, check_boundary);
    rr = and_then(rr, add_one);
    finally(rr, on_ok, on_err);
    return 0;
}
