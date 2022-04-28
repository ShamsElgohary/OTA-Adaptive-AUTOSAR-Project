
#include <requests>





URL='http://127.0.0.1:8000/download/'
res = requests.get(url = URL)

print(res.text)
