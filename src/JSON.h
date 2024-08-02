/* Downloaded from https://repo.progsbase.com - Code Developed Using progsbase. */

#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>
#include <wchar.h>

#define strparam(str) ((wchar_t *)str), wcslen(str)

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

struct ElementArrayReference;
typedef struct ElementArrayReference ElementArrayReference;

struct LinkedListElements;
typedef struct LinkedListElements LinkedListElements;

struct LinkedListNodeElements;
typedef struct LinkedListNodeElements LinkedListNodeElements;

struct Element;
typedef struct Element Element;

struct ElementReference;
typedef struct ElementReference ElementReference;

struct ElementType;
typedef struct ElementType ElementType;

struct StringElementMap;
typedef struct StringElementMap StringElementMap;

struct Example;
typedef struct Example Example;

struct X;
typedef struct X X;

struct BooleanArrayReference;
typedef struct BooleanArrayReference BooleanArrayReference;

struct BooleanReference;
typedef struct BooleanReference BooleanReference;

struct CharacterReference;
typedef struct CharacterReference CharacterReference;

struct NumberArrayReference;
typedef struct NumberArrayReference NumberArrayReference;

struct NumberReference;
typedef struct NumberReference NumberReference;

struct StringArrayReference;
typedef struct StringArrayReference StringArrayReference;

struct StringReference;
typedef struct StringReference StringReference;

struct DynamicArrayCharacters;
typedef struct DynamicArrayCharacters DynamicArrayCharacters;

struct LinkedListNodeStrings;
typedef struct LinkedListNodeStrings LinkedListNodeStrings;

struct LinkedListStrings;
typedef struct LinkedListStrings LinkedListStrings;

struct LinkedListNodeNumbers;
typedef struct LinkedListNodeNumbers LinkedListNodeNumbers;

struct LinkedListNumbers;
typedef struct LinkedListNumbers LinkedListNumbers;

struct LinkedListCharacters;
typedef struct LinkedListCharacters LinkedListCharacters;

struct LinkedListNodeCharacters;
typedef struct LinkedListNodeCharacters LinkedListNodeCharacters;

struct DynamicArrayNumbers;
typedef struct DynamicArrayNumbers DynamicArrayNumbers;

struct ElementArrayReference{
  Element **array;
  size_t arrayLength;
};

struct LinkedListElements{
  LinkedListNodeElements *first;
  LinkedListNodeElements *last;
};

struct LinkedListNodeElements{
  _Bool end;
  Element *value;
  LinkedListNodeElements *next;
};

struct Element{
  wchar_t *type;
  size_t typeLength;
  StringElementMap *object;
  Element **array;
  size_t arrayLength;
  wchar_t *string;
  size_t stringLength;
  double number;
  _Bool booleanValue;
};

struct ElementReference{
  Element *element;
};

struct ElementType{
  wchar_t *name;
  size_t nameLength;
};

struct StringElementMap{
  StringArrayReference *stringListRef;
  ElementArrayReference *elementListRef;
};

struct Example{
  wchar_t *a;
  size_t aLength;
  double *b;
  size_t bLength;
  X *x;
};

struct X{
  wchar_t *x1;
  size_t x1Length;
  _Bool x1IsNull;
  _Bool x2;
  _Bool x3;
};

struct BooleanArrayReference{
  _Bool *booleanArray;
  size_t booleanArrayLength;
};

struct BooleanReference{
  _Bool booleanValue;
};

struct CharacterReference{
  wchar_t characterValue;
};

struct NumberArrayReference{
  double *numberArray;
  size_t numberArrayLength;
};

struct NumberReference{
  double numberValue;
};

struct StringArrayReference{
  StringReference **stringArray;
  size_t stringArrayLength;
};

struct StringReference{
  wchar_t *string;
  size_t stringLength;
};

struct DynamicArrayCharacters{
  wchar_t *array;
  size_t arrayLength;
  double length;
};

struct LinkedListNodeStrings{
  _Bool end;
  wchar_t *value;
  size_t valueLength;
  LinkedListNodeStrings *next;
};

struct LinkedListStrings{
  LinkedListNodeStrings *first;
  LinkedListNodeStrings *last;
};

struct LinkedListNodeNumbers{
  LinkedListNodeNumbers *next;
  _Bool end;
  double value;
};

struct LinkedListNumbers{
  LinkedListNodeNumbers *first;
  LinkedListNodeNumbers *last;
};

struct LinkedListCharacters{
  LinkedListNodeCharacters *first;
  LinkedListNodeCharacters *last;
};

struct LinkedListNodeCharacters{
  _Bool end;
  wchar_t value;
  LinkedListNodeCharacters *next;
};

struct DynamicArrayNumbers{
  double *array;
  size_t arrayLength;
  double length;
};

wchar_t *GetWC(char *c);
_Bool IsValidJSON(wchar_t *json, size_t jsonLength, StringArrayReference *errorMessage);
_Bool JSONTokenize(wchar_t *json, size_t jsonLength, StringArrayReference *tokensReference, StringArrayReference *errorMessages);
_Bool GetJSONNumberToken(wchar_t *json, size_t jsonLength, double start, StringReference *tokenReference, StringArrayReference *errorMessages);
_Bool IsValidJSONNumber(wchar_t *n, size_t nLength, StringArrayReference *errorMessages);
_Bool IsValidJSONNumberAfterSign(wchar_t *n, size_t nLength, double i, StringArrayReference *errorMessages);
double IsValidJSONNumberAdvancePastDigits(wchar_t *n, size_t nLength, double i);
_Bool IsValidJSONNumberFromDotOrExponent(wchar_t *n, size_t nLength, double i, StringArrayReference *errorMessages);
_Bool IsValidJSONNumberFromExponent(wchar_t *n, size_t nLength, double i, StringArrayReference *errorMessages);
_Bool IsJSONNumberCharacter(wchar_t c);
_Bool GetJSONPrimitiveName(wchar_t *string, size_t stringLength, double start, StringArrayReference *errorMessages, wchar_t *primitive, size_t primitiveLength, StringReference *tokenReference);
_Bool GetJSONString(wchar_t *json, size_t jsonLength, double start, StringReference *tokenReference, NumberReference *stringLengthReference, StringArrayReference *errorMessages);
_Bool IsValidJSONString(wchar_t *jsonString, size_t jsonStringLength, StringArrayReference *errorMessages);
_Bool IsValidJSONStringInJSON(wchar_t *json, size_t jsonLength, double start, NumberReference *characterCount, NumberReference *stringLengthReference, StringArrayReference *errorMessages);
_Bool IsJSONIllegalControllCharacter(wchar_t c);

Element **AddElement(size_t *returnArrayLength, Element **list, size_t listLength, Element *a);
void AddElementRef(ElementArrayReference *list, Element *i);
Element **RemoveElement(size_t *returnArrayLength, Element **list, size_t listLength, double n);
Element *GetElementRef(ElementArrayReference *list, double i);
void RemoveElementRef(ElementArrayReference *list, double i);


LinkedListElements *CreateLinkedListElements();
void LinkedListAddElement(LinkedListElements *ll, Element *value);
Element **LinkedListElementsToArray(size_t *returnArrayLength, LinkedListElements *ll);
double LinkedListElementsLength(LinkedListElements *ll);
void FreeLinkedListElements(LinkedListElements *ll);


double ComputeJSONStringLength(Element *element);
double ComputeJSONBooleanStringLength(Element *element);
double ComputeJSONNumberStringLength(Element *element);
double ComputeJSONArrayStringLength(Element *element);
double ComputeJSONObjectStringLength(Element *element);

Element *CreateStringElement(wchar_t *string, size_t stringLength);
Element *CreateBooleanElement(_Bool booleanValue);
Element *CreateNullElement();
Element *CreateNumberElement(double number);
Element *CreateArrayElement(double length);
Element *CreateObjectElement(double length);
void DeleteElement(Element *element);
void DeleteObject(Element *element);
void DeleteArray(Element *element);

wchar_t *WriteJSON(size_t *returnArrayLength, Element *element);
void WriteBooleanValue(Element *element, wchar_t *result, size_t resultLength, NumberReference *index);
void WriteNumber(Element *element, wchar_t *result, size_t resultLength, NumberReference *index);
void WriteArray(Element *element, wchar_t *result, size_t resultLength, NumberReference *index);
void WriteString(Element *element, wchar_t *result, size_t resultLength, NumberReference *index);
wchar_t *JSONEscapeString(size_t *returnArrayLength, wchar_t *string, size_t stringLength);
double JSONEscapedStringLength(wchar_t *string, size_t stringLength);
wchar_t *JSONEscapeCharacter(size_t *returnArrayLength, wchar_t c);
_Bool JSONMustBeEscaped(wchar_t c, NumberReference *letters);
void WriteObject(Element *element, wchar_t *result, size_t resultLength, NumberReference *index);

_Bool ReadJSON(wchar_t *string, size_t stringLength, ElementReference *elementReference, StringArrayReference *errorMessages);
_Bool GetJSONValue(StringReference **tokens, size_t tokensLength, ElementReference *elementReference, StringArrayReference *errorMessages);
_Bool GetJSONValueRecursive(StringReference **tokens, size_t tokensLength, NumberReference *i, double depth, ElementReference *elementReference, StringArrayReference *errorMessages);
_Bool GetJSONObject(StringReference **tokens, size_t tokensLength, NumberReference *i, double depth, ElementReference *elementReference, StringArrayReference *errorMessages);
_Bool GetJSONArray(StringReference **tokens, size_t tokensLength, NumberReference *i, double depth, ElementReference *elementReference, StringArrayReference *errorMessages);

StringArrayReference *GetStringElementMapKeySet(StringElementMap *stringElementMap);
Element *GetObjectValue(StringElementMap *stringElementMap, wchar_t *key, size_t keyLength);
Element *GetObjectValueWithCheck(StringElementMap *stringElementMap, wchar_t *key, size_t keyLength, BooleanReference *foundReference);
void PutStringElementMap(StringElementMap *stringElementMap, wchar_t *keystring, size_t keystringLength, Element *value);
void SetStringElementMap(StringElementMap *stringElementMap, double index, wchar_t *keystring, size_t keystringLength, Element *value);
double GetStringElementMapNumberOfKeys(StringElementMap *stringElementMap);

_Bool JSONCompare(wchar_t *a, size_t aLength, wchar_t *b, size_t bLength, double epsilon, BooleanReference *equal, StringArrayReference *errorMessage);
_Bool JSONCompareElements(Element *ea, Element *eb, double epsilon);
_Bool JSONCompareArrays(Element **ea, size_t eaLength, Element **eb, size_t ebLength, double epsilon);
_Bool JSONCompareObjects(StringElementMap *ea, StringElementMap *eb, double epsilon);

void testEscaper(NumberReference *failures);

Example *mapTo(Element *root);
X *mapXTo(StringElementMap *object);
double *mapbTo(size_t *returnArrayLength, Element **array, size_t arrayLength);

void testWriter(NumberReference *failures);
Element *createExampleJSON();
void testWriterEscape(NumberReference *failures);
void testWriter2(NumberReference *failures);

void testReader(NumberReference *failures);
void test2(NumberReference *failures);
void testReaderExample(NumberReference *failures);
void JSONExample(wchar_t *json, size_t jsonLength, StringArrayReference *errorMessages, ElementReference *elementReference, StringReference *outputJSON);

double test();
void testValidator(NumberReference *failures);
void testComparator(NumberReference *failures);

void testTokenizer1(NumberReference *failures);

BooleanReference *CreateBooleanReference(_Bool value);
BooleanArrayReference *CreateBooleanArrayReference(_Bool *value, size_t valueLength);
BooleanArrayReference *CreateBooleanArrayReferenceLengthValue(double length, _Bool value);
void FreeBooleanArrayReference(BooleanArrayReference *booleanArrayReference);
CharacterReference *CreateCharacterReference(wchar_t value);
NumberReference *CreateNumberReference(double value);
NumberArrayReference *CreateNumberArrayReference(double *value, size_t valueLength);
NumberArrayReference *CreateNumberArrayReferenceLengthValue(double length, double value);
void FreeNumberArrayReference(NumberArrayReference *numberArrayReference);
StringReference *CreateStringReference(wchar_t *value, size_t valueLength);
StringReference *CreateStringReferenceLengthValue(double length, wchar_t value);
void FreeStringReference(StringReference *stringReference);
StringArrayReference *CreateStringArrayReference(StringReference **strings, size_t stringsLength);
StringArrayReference *CreateStringArrayReferenceLengthValue(double length, wchar_t *value, size_t valueLength);
void FreeStringArrayReference(StringArrayReference *stringArrayReference);

void strWriteStringToStingStream(wchar_t *stream, size_t streamLength, NumberReference *index, wchar_t *src, size_t srcLength);
void strWriteCharacterToStingStream(wchar_t *stream, size_t streamLength, NumberReference *index, wchar_t src);
void strWriteBooleanToStingStream(wchar_t *stream, size_t streamLength, NumberReference *index, _Bool src);

_Bool strSubstringWithCheck(wchar_t *string, size_t stringLength, double from, double to, StringReference *stringReference);
wchar_t *strSubstring(size_t *returnArrayLength, wchar_t *string, size_t stringLength, double from, double to);
wchar_t *strAppendString(size_t *returnArrayLength, wchar_t *s1, size_t s1Length, wchar_t *s2, size_t s2Length);
wchar_t *strConcatenateString(size_t *returnArrayLength, wchar_t *s1, size_t s1Length, wchar_t *s2, size_t s2Length);
wchar_t *strAppendCharacter(size_t *returnArrayLength, wchar_t *string, size_t stringLength, wchar_t c);
wchar_t *strConcatenateCharacter(size_t *returnArrayLength, wchar_t *string, size_t stringLength, wchar_t c);
StringReference **strSplitByCharacter(size_t *returnArrayLength, wchar_t *toSplit, size_t toSplitLength, wchar_t splitBy);
_Bool strIndexOfCharacter(wchar_t *string, size_t stringLength, wchar_t character, NumberReference *indexReference);
_Bool strSubstringEqualsWithCheck(wchar_t *string, size_t stringLength, double from, wchar_t *substring, size_t substringLength, BooleanReference *equalsReference);
_Bool strSubstringEquals(wchar_t *string, size_t stringLength, double from, wchar_t *substring, size_t substringLength);
_Bool strIndexOfString(wchar_t *string, size_t stringLength, wchar_t *substring, size_t substringLength, NumberReference *indexReference);
_Bool strContainsCharacter(wchar_t *string, size_t stringLength, wchar_t character);
_Bool strContainsString(wchar_t *string, size_t stringLength, wchar_t *substring, size_t substringLength);
void strToUpperCase(wchar_t *string, size_t stringLength);
void strToLowerCase(wchar_t *string, size_t stringLength);
_Bool strEqualsIgnoreCase(wchar_t *a, size_t aLength, wchar_t *b, size_t bLength);
wchar_t *strReplaceString(size_t *returnArrayLength, wchar_t *string, size_t stringLength, wchar_t *toReplace, size_t toReplaceLength, wchar_t *replaceWith, size_t replaceWithLength);
wchar_t *strReplaceCharacterToNew(size_t *returnArrayLength, wchar_t *string, size_t stringLength, wchar_t toReplace, wchar_t replaceWith);
void strReplaceCharacter(wchar_t *string, size_t stringLength, wchar_t toReplace, wchar_t replaceWith);
wchar_t *strTrim(size_t *returnArrayLength, wchar_t *string, size_t stringLength);
_Bool strStartsWith(wchar_t *string, size_t stringLength, wchar_t *start, size_t startLength);
_Bool strEndsWith(wchar_t *string, size_t stringLength, wchar_t *end, size_t endLength);
StringReference **strSplitByString(size_t *returnArrayLength, wchar_t *toSplit, size_t toSplitLength, wchar_t *splitBy, size_t splitByLength);
_Bool strStringIsBefore(wchar_t *a, size_t aLength, wchar_t *b, size_t bLength);
wchar_t *strJoinStringsWithSeparator(size_t *returnArrayLength, StringReference **strings, size_t stringsLength, wchar_t *separator, size_t separatorLength);
wchar_t *strJoinStrings(size_t *returnArrayLength, StringReference **strings, size_t stringsLength);

wchar_t *nCreateStringScientificNotationDecimalFromNumber(size_t *returnArrayLength, double decimal);
wchar_t *nCreateStringDecimalFromNumber(size_t *returnArrayLength, double decimal);
_Bool nCreateStringFromNumberWithCheck(double decimal, double base, StringReference *stringReference);
double nGetMaximumDigitsForBase(double base);
double nGetFirstDigitPosition(double decimal, double base);
_Bool nGetSingleDigitCharacterFromNumberWithCheck(double c, double base, CharacterReference *characterReference);
wchar_t *nGetDigitCharacterTable(size_t *returnArrayLength);

_Bool nCreateNumberFromDecimalStringWithCheck(wchar_t *string, size_t stringLength, NumberReference *decimalReference, StringReference *errorMessage);
double nCreateNumberFromDecimalString(wchar_t *string, size_t stringLength);
_Bool nCreateNumberFromStringWithCheck(wchar_t *string, size_t stringLength, double base, NumberReference *numberReference, StringReference *errorMessage);
double nCreateNumberFromParts(double base, _Bool numberIsPositive, double *beforePoint, size_t beforePointLength, double *afterPoint, size_t afterPointLength, _Bool exponentIsPositive, double *exponent, size_t exponentLength);
_Bool nExtractPartsFromNumberString(wchar_t *n, size_t nLength, double base, BooleanReference *numberIsPositive, NumberArrayReference *beforePoint, NumberArrayReference *afterPoint, BooleanReference *exponentIsPositive, NumberArrayReference *exponent, StringReference *errorMessages);
double nGetNumberFromNumberCharacterForBase(wchar_t c, double base);
_Bool nCharacterIsNumberCharacterInBase(wchar_t c, double base);
double *nStringToNumberArray(size_t *returnArrayLength, wchar_t *str, size_t strLength);
_Bool nStringToNumberArrayWithCheck(wchar_t *str, size_t strLength, NumberArrayReference *numberArrayReference, StringReference *errorMessage);

double *AddNumber(size_t *returnArrayLength, double *list, size_t listLength, double a);
void AddNumberRef(NumberArrayReference *list, double i);
double *RemoveNumber(size_t *returnArrayLength, double *list, size_t listLength, double n);
double GetNumberRef(NumberArrayReference *list, double i);
void RemoveNumberRef(NumberArrayReference *list, double i);

StringReference **AddString(size_t *returnArrayLength, StringReference **list, size_t listLength, StringReference *a);
void AddStringRef(StringArrayReference *list, StringReference *i);
StringReference **RemoveString(size_t *returnArrayLength, StringReference **list, size_t listLength, double n);
StringReference *GetStringRef(StringArrayReference *list, double i);
void RemoveStringRef(StringArrayReference *list, double i);


DynamicArrayCharacters *CreateDynamicArrayCharacters();
DynamicArrayCharacters *CreateDynamicArrayCharactersWithInitialCapacity(double capacity);
void DynamicArrayAddCharacter(DynamicArrayCharacters *da, wchar_t value);
void DynamicArrayCharactersIncreaseSize(DynamicArrayCharacters *da);
_Bool DynamicArrayCharactersDecreaseSizeNecessary(DynamicArrayCharacters *da);
void DynamicArrayCharactersDecreaseSize(DynamicArrayCharacters *da);
double DynamicArrayCharactersIndex(DynamicArrayCharacters *da, double index);
double DynamicArrayCharactersLength(DynamicArrayCharacters *da);
void DynamicArrayInsertCharacter(DynamicArrayCharacters *da, double index, wchar_t value);
_Bool DynamicArrayCharacterSet(DynamicArrayCharacters *da, double index, wchar_t value);
void DynamicArrayRemoveCharacter(DynamicArrayCharacters *da, double index);
void FreeDynamicArrayCharacters(DynamicArrayCharacters *da);
wchar_t *DynamicArrayCharactersToArray(size_t *returnArrayLength, DynamicArrayCharacters *da);
DynamicArrayCharacters *ArrayToDynamicArrayCharactersWithOptimalSize(wchar_t *array, size_t arrayLength);
DynamicArrayCharacters *ArrayToDynamicArrayCharacters(wchar_t *array, size_t arrayLength);
_Bool DynamicArrayCharactersEqual(DynamicArrayCharacters *a, DynamicArrayCharacters *b);
LinkedListCharacters *DynamicArrayCharactersToLinkedList(DynamicArrayCharacters *da);
DynamicArrayCharacters *LinkedListToDynamicArrayCharacters(LinkedListCharacters *ll);

_Bool *AddBoolean(size_t *returnArrayLength, _Bool *list, size_t listLength, _Bool a);
void AddBooleanRef(BooleanArrayReference *list, _Bool i);
_Bool *RemoveBoolean(size_t *returnArrayLength, _Bool *list, size_t listLength, double n);
_Bool GetBooleanRef(BooleanArrayReference *list, double i);
void RemoveDecimalRef(BooleanArrayReference *list, double i);


LinkedListStrings *CreateLinkedListString();
void LinkedListAddString(LinkedListStrings *ll, wchar_t *value, size_t valueLength);
StringReference **LinkedListStringsToArray(size_t *returnArrayLength, LinkedListStrings *ll);
double LinkedListStringsLength(LinkedListStrings *ll);
void FreeLinkedListString(LinkedListStrings *ll);


LinkedListNumbers *CreateLinkedListNumbers();
LinkedListNumbers **CreateLinkedListNumbersArray(size_t *returnArrayLength, double length);
void LinkedListAddNumber(LinkedListNumbers *ll, double value);
double LinkedListNumbersLength(LinkedListNumbers *ll);
double LinkedListNumbersIndex(LinkedListNumbers *ll, double index);
void LinkedListInsertNumber(LinkedListNumbers *ll, double index, double value);
void LinkedListSet(LinkedListNumbers *ll, double index, double value);
void LinkedListRemoveNumber(LinkedListNumbers *ll, double index);
void FreeLinkedListNumbers(LinkedListNumbers *ll);
void FreeLinkedListNumbersArray(LinkedListNumbers **lls, size_t llsLength);
double *LinkedListNumbersToArray(size_t *returnArrayLength, LinkedListNumbers *ll);
LinkedListNumbers *ArrayToLinkedListNumbers(double *array, size_t arrayLength);
_Bool LinkedListNumbersEqual(LinkedListNumbers *a, LinkedListNumbers *b);

LinkedListCharacters *CreateLinkedListCharacter();
void LinkedListAddCharacter(LinkedListCharacters *ll, wchar_t value);
wchar_t *LinkedListCharactersToArray(size_t *returnArrayLength, LinkedListCharacters *ll);
double LinkedListCharactersLength(LinkedListCharacters *ll);
void FreeLinkedListCharacter(LinkedListCharacters *ll);
void LinkedListCharactersAddString(LinkedListCharacters *ll, wchar_t *str, size_t strLength);



DynamicArrayNumbers *CreateDynamicArrayNumbers();
DynamicArrayNumbers *CreateDynamicArrayNumbersWithInitialCapacity(double capacity);
void DynamicArrayAddNumber(DynamicArrayNumbers *da, double value);
void DynamicArrayNumbersIncreaseSize(DynamicArrayNumbers *da);
_Bool DynamicArrayNumbersDecreaseSizeNecessary(DynamicArrayNumbers *da);
void DynamicArrayNumbersDecreaseSize(DynamicArrayNumbers *da);
double DynamicArrayNumbersIndex(DynamicArrayNumbers *da, double index);
double DynamicArrayNumbersLength(DynamicArrayNumbers *da);
void DynamicArrayInsertNumber(DynamicArrayNumbers *da, double index, double value);
_Bool DynamicArrayNumberSet(DynamicArrayNumbers *da, double index, double value);
void DynamicArrayRemoveNumber(DynamicArrayNumbers *da, double index);
void FreeDynamicArrayNumbers(DynamicArrayNumbers *da);
double *DynamicArrayNumbersToArray(size_t *returnArrayLength, DynamicArrayNumbers *da);
DynamicArrayNumbers *ArrayToDynamicArrayNumbersWithOptimalSize(double *array, size_t arrayLength);
DynamicArrayNumbers *ArrayToDynamicArrayNumbers(double *array, size_t arrayLength);
_Bool DynamicArrayNumbersEqual(DynamicArrayNumbers *a, DynamicArrayNumbers *b);
LinkedListNumbers *DynamicArrayNumbersToLinkedList(DynamicArrayNumbers *da);
DynamicArrayNumbers *LinkedListToDynamicArrayNumbers(LinkedListNumbers *ll);
double DynamicArrayNumbersIndexOf(DynamicArrayNumbers *arr, double n, BooleanReference *foundReference);
_Bool DynamicArrayNumbersIsInArray(DynamicArrayNumbers *arr, double n);

wchar_t *AddCharacter(size_t *returnArrayLength, wchar_t *list, size_t listLength, wchar_t a);
void AddCharacterRef(StringReference *list, wchar_t i);
wchar_t *RemoveCharacter(size_t *returnArrayLength, wchar_t *list, size_t listLength, double n);
wchar_t GetCharacterRef(StringReference *list, double i);
void RemoveCharacterRef(StringReference *list, double i);

double Negate(double x);
double Positive(double x);
double Factorial(double x);
double Round(double x);
double BankersRound(double x);
double Ceil(double x);
double Floor(double x);
double Truncate(double x);
double Absolute(double x);
double Logarithm(double x);
double NaturalLogarithm(double x);
double Sin(double x);
double Cos(double x);
double Tan(double x);
double Asin(double x);
double Acos(double x);
double Atan(double x);
double Atan2(double y, double x);
double Squareroot(double x);
double Exp(double x);
_Bool DivisibleBy(double a, double b);
double Combinations(double n, double k);
double Permutations(double n, double k);
_Bool EpsilonCompare(double a, double b, double epsilon);
double GreatestCommonDivisor(double a, double b);
double GCDWithSubtraction(double a, double b);
_Bool IsInteger(double a);
_Bool GreatestCommonDivisorWithCheck(double a, double b, NumberReference *gcdReference);
double LeastCommonMultiple(double a, double b);
double Sign(double a);
double Max(double a, double b);
double Min(double a, double b);
double Power(double a, double b);
double Gamma(double x);
double LogGamma(double x);
double LanczosApproximation(double z);
double Beta(double x, double y);
double Sinh(double x);
double Cosh(double x);
double Tanh(double x);
double Cot(double x);
double Sec(double x);
double Csc(double x);
double Coth(double x);
double Sech(double x);
double Csch(double x);
double Error(double x);
double ErrorInverse(double x);
double FallingFactorial(double x, double n);
double RisingFactorial(double x, double n);
double Hypergeometric(double a, double b, double c, double z, double maxIterations, double precision);
double HypergeometricDirect(double a, double b, double c, double z, double maxIterations, double precision);
double BernouilliNumber(double n);
double AkiyamaTanigawaAlgorithm(double n);

double *StringToNumberArray(size_t *returnArrayLength, wchar_t *string, size_t stringLength);
wchar_t *NumberArrayToString(size_t *returnArrayLength, double *array, size_t arrayLength);
_Bool NumberArraysEqual(double *a, size_t aLength, double *b, size_t bLength);
_Bool BooleanArraysEqual(_Bool *a, size_t aLength, _Bool *b, size_t bLength);
_Bool StringsEqual(wchar_t *a, size_t aLength, wchar_t *b, size_t bLength);
void FillNumberArray(double *a, size_t aLength, double value);
void FillString(wchar_t *a, size_t aLength, wchar_t value);
void FillBooleanArray(_Bool *a, size_t aLength, _Bool value);
_Bool FillNumberArrayRange(double *a, size_t aLength, double value, double from, double to);
_Bool FillBooleanArrayRange(_Bool *a, size_t aLength, _Bool value, double from, double to);
_Bool FillStringRange(wchar_t *a, size_t aLength, wchar_t value, double from, double to);
double *CopyNumberArray(size_t *returnArrayLength, double *a, size_t aLength);
_Bool *CopyBooleanArray(size_t *returnArrayLength, _Bool *a, size_t aLength);
wchar_t *CopyString(size_t *returnArrayLength, wchar_t *a, size_t aLength);
_Bool CopyNumberArrayRange(double *a, size_t aLength, double from, double to, NumberArrayReference *copyReference);
_Bool CopyBooleanArrayRange(_Bool *a, size_t aLength, double from, double to, BooleanArrayReference *copyReference);
_Bool CopyStringRange(wchar_t *a, size_t aLength, double from, double to, StringReference *copyReference);
_Bool IsLastElement(double length, double index);
double *CreateNumberArray(size_t *returnArrayLength, double length, double value);
_Bool *CreateBooleanArray(size_t *returnArrayLength, double length, _Bool value);
wchar_t *CreateString(size_t *returnArrayLength, double length, wchar_t value);
void SwapElementsOfNumberArray(double *A, size_t ALength, double ai, double bi);
void SwapElementsOfStringArray(StringArrayReference *A, double ai, double bi);
void ReverseNumberArray(double *array, size_t arrayLength);

wchar_t charToLowerCase(wchar_t character);
wchar_t charToUpperCase(wchar_t character);
_Bool charIsUpperCase(wchar_t character);
_Bool charIsLowerCase(wchar_t character);
_Bool charIsLetter(wchar_t character);
_Bool charIsNumber(wchar_t character);
_Bool charIsWhiteSpace(wchar_t character);
_Bool charIsSymbol(wchar_t character);
_Bool charCharacterIsBefore(wchar_t a, wchar_t b);
wchar_t charDecimalDigitToCharacter(double digit);
double charCharacterToDecimalDigit(wchar_t c);

void AssertFalse(_Bool b, NumberReference *failures);
void AssertTrue(_Bool b, NumberReference *failures);
void AssertEquals(double a, double b, NumberReference *failures);
void AssertBooleansEqual(_Bool a, _Bool b, NumberReference *failures);
void AssertCharactersEqual(wchar_t a, wchar_t b, NumberReference *failures);
void AssertStringEquals(wchar_t *a, size_t aLength, wchar_t *b, size_t bLength, NumberReference *failures);
void AssertNumberArraysEqual(double *a, size_t aLength, double *b, size_t bLength, NumberReference *failures);
void AssertBooleanArraysEqual(_Bool *a, size_t aLength, _Bool *b, size_t bLength, NumberReference *failures);
void AssertStringArraysEqual(StringReference **a, size_t aLength, StringReference **b, size_t bLength, NumberReference *failures);

wchar_t *GetWC(char *c)
{
  size_t cSize = strlen(c)+1;
  wchar_t* wc = (wchar_t*)malloc(cSize);
  mbstowcs (wc, c, cSize);
  /*swprintf(wc, strlen(c), L"%s", c);*/
  return wc;
}
_Bool IsValidJSON(wchar_t *json, size_t jsonLength, StringArrayReference *errorMessage){
  _Bool success;
  ElementReference *elementReference;

  elementReference = (ElementReference *)malloc(sizeof(ElementReference));

  success = ReadJSON(json, jsonLength, elementReference, errorMessage);

  if(success){
    DeleteElement(elementReference->element);
  }

  return success;
}
_Bool JSONTokenize(wchar_t *json, size_t jsonLength, StringArrayReference *tokensReference, StringArrayReference *errorMessages){
  double i;
  wchar_t c;
  wchar_t *str;
  size_t strLength;
  StringReference *stringReference, *tokenReference;
  NumberReference *stringLength;
  _Bool success;
  LinkedListStrings *ll;

  ll = CreateLinkedListString();
  success = true;

  stringLength = (NumberReference *)malloc(sizeof(NumberReference));
  tokenReference = (StringReference *)malloc(sizeof(StringReference));

  for(i = 0.0; i < jsonLength && success; ){
    c = json[(int)(i)];

    if(c == '{'){
      LinkedListAddString(ll, strparam(L"{"));
      i = i + 1.0;
    }else if(c == '}'){
      LinkedListAddString(ll, strparam(L"}"));
      i = i + 1.0;
    }else if(c == '['){
      LinkedListAddString(ll, strparam(L"["));
      i = i + 1.0;
    }else if(c == ']'){
      LinkedListAddString(ll, strparam(L"]"));
      i = i + 1.0;
    }else if(c == ':'){
      LinkedListAddString(ll, strparam(L":"));
      i = i + 1.0;
    }else if(c == ','){
      LinkedListAddString(ll, strparam(L","));
      i = i + 1.0;
    }else if(c == 'f'){
      success = GetJSONPrimitiveName(json, jsonLength, i, errorMessages, strparam(L"false"), tokenReference);
      if(success){
        LinkedListAddString(ll, strparam(L"false"));
        i = i + wcslen(L"false");
      }
    }else if(c == 't'){
      success = GetJSONPrimitiveName(json, jsonLength, i, errorMessages, strparam(L"true"), tokenReference);
      if(success){
        LinkedListAddString(ll, strparam(L"true"));
        i = i + wcslen(L"true");
      }
    }else if(c == 'n'){
      success = GetJSONPrimitiveName(json, jsonLength, i, errorMessages, strparam(L"null"), tokenReference);
      if(success){
        //wchar_t *null = (wchar_t*)malloc(sizeof(wchar_t) * (wcslen(L"null")));
        //wcscpy(null, L"null");
        LinkedListAddString(ll, strparam(L"null"));
        i = i + wcslen(L"null");
      }
    }else if(c == ' ' || c == '\n' || c == '\t' || c == '\r'){
      /* Skip. */
      i = i + 1.0;
    }else if(c == '\"'){
      success = GetJSONString(json, jsonLength, i, tokenReference, stringLength, errorMessages);
      if(success){
        LinkedListAddString(ll, tokenReference->string, tokenReference->stringLength);
        i = i + stringLength->numberValue;
        //free(tokenReference->string);
      }
    }else if(IsJSONNumberCharacter(c)){
      success = GetJSONNumberToken(json, jsonLength, i, tokenReference, errorMessages);
      if(success){
        LinkedListAddString(ll, tokenReference->string, tokenReference->stringLength);
        i = i + tokenReference->stringLength;
      }
      //free(tokenReference->string);
    }else{
      str = strConcatenateCharacter(&strLength, strparam(L"Invalid start of Token: "), c);
      stringReference = CreateStringReference(str, strLength);
      AddStringRef(errorMessages, stringReference);
      i = i + 1.0;
      success = false;
    }
  }

  if(success){
    LinkedListAddString(ll, strparam(L"<end>"));
    tokensReference->stringArray = LinkedListStringsToArray(&tokensReference->stringArrayLength, ll);
    FreeLinkedListString(ll);
  }
  free(stringLength);
  free(tokenReference);

  return success;
}
_Bool GetJSONNumberToken(wchar_t *json, size_t jsonLength, double start, StringReference *tokenReference, StringArrayReference *errorMessages){
  wchar_t c;
  double end, i;
  _Bool done, success;
  wchar_t *numberString;
  size_t numberStringLength;

  end = jsonLength;
  done = false;

  for(i = start; i < jsonLength &&  !done ; i = i + 1.0){
    c = json[(int)(i)];
    if( !IsJSONNumberCharacter(c) ){
      done = true;
      end = i;
    }
  }

  numberString = strSubstring(&numberStringLength, json, jsonLength, start, end);

  success = IsValidJSONNumber(numberString, numberStringLength, errorMessages);

  tokenReference->string = numberString;
  tokenReference->stringLength = numberStringLength;

  return success;
}
_Bool IsValidJSONNumber(wchar_t *n, size_t nLength, StringArrayReference *errorMessages){
  _Bool success;
  double i;

  i = 0.0;

  /* JSON allows an optional negative sign. */
  if(n[(int)(i)] == '-'){
    i = i + 1.0;
  }

  if(i < nLength){
    success = IsValidJSONNumberAfterSign(n, nLength, i, errorMessages);
  }else{
    success = false;
    AddStringRef(errorMessages, CreateStringReference(strparam(L"Number must contain at least one digit.")));
  }

  return success;
}
_Bool IsValidJSONNumberAfterSign(wchar_t *n, size_t nLength, double i, StringArrayReference *errorMessages){
  _Bool success;

  if(charIsNumber(n[(int)(i)])){
    /* 0 first means only 0. */
    if(n[(int)(i)] == '0'){
      i = i + 1.0;
    }else{
      /* 1-9 first, read following digits. */
      i = IsValidJSONNumberAdvancePastDigits(n, nLength, i);
    }

    if(i < nLength){
      success = IsValidJSONNumberFromDotOrExponent(n, nLength, i, errorMessages);
    }else{
      /* If integer, we are done now. */
      success = true;
    }
  }else{
    success = false;
    AddStringRef(errorMessages, CreateStringReference(strparam(L"A number must start with 0-9 (after the optional sign).")));
  }

  return success;
}
double IsValidJSONNumberAdvancePastDigits(wchar_t *n, size_t nLength, double i){
  _Bool done;

  i = i + 1.0;
  done = false;
  for(; i < nLength &&  !done ; ){
    if(charIsNumber(n[(int)(i)])){
      i = i + 1.0;
    }else{
      done = true;
    }
  }

  return i;
}
_Bool IsValidJSONNumberFromDotOrExponent(wchar_t *n, size_t nLength, double i, StringArrayReference *errorMessages){
  _Bool wasDotAndOrE, success;

  wasDotAndOrE = false;
  success = true;

  if(n[(int)(i)] == '.'){
    i = i + 1.0;
    wasDotAndOrE = true;

    if(i < nLength){
      if(charIsNumber(n[(int)(i)])){
        /* Read digits following decimal point. */
        i = IsValidJSONNumberAdvancePastDigits(n, nLength, i);

        if(i == nLength){
          /* If non-scientific decimal number, we are done. */
          success = true;
        }
      }else{
        success = false;
        AddStringRef(errorMessages, CreateStringReference(strparam(L"There must be numbers after the decimal point.")));
      }
    }else{
      success = false;
      AddStringRef(errorMessages, CreateStringReference(strparam(L"There must be numbers after the decimal point.")));
    }
  }

  if(i < nLength && success){
    if(n[(int)(i)] == 'e' || n[(int)(i)] == 'E'){
      wasDotAndOrE = true;
      success = IsValidJSONNumberFromExponent(n, nLength, i, errorMessages);
    }else{
      success = false;
      AddStringRef(errorMessages, CreateStringReference(strparam(L"Expected e or E.")));
    }
  }else if(i == nLength && success){
    /* If number with decimal point. */
    success = true;
  }else{
    success = false;
    AddStringRef(errorMessages, CreateStringReference(strparam(L"There must be numbers after the decimal point.")));
  }

  if(wasDotAndOrE){
  }else{
    success = false;
    AddStringRef(errorMessages, CreateStringReference(strparam(L"Exprected decimal point or e or E.")));
  }

  return success;
}
_Bool IsValidJSONNumberFromExponent(wchar_t *n, size_t nLength, double i, StringArrayReference *errorMessages){
  _Bool success;

  i = i + 1.0;

  if(i < nLength){
    /* The exponent sign can either + or -, */
    if(n[(int)(i)] == '+' || n[(int)(i)] == '-'){
      i = i + 1.0;
    }

    if(i < nLength){
      if(charIsNumber(n[(int)(i)])){
        /* Read digits following decimal point. */
        i = IsValidJSONNumberAdvancePastDigits(n, nLength, i);

        if(i == nLength){
          /* We found scientific number. */
          success = true;
        }else{
          success = false;
          AddStringRef(errorMessages, CreateStringReference(strparam(L"There was characters following the exponent.")));
        }
      }else{
        success = false;
        AddStringRef(errorMessages, CreateStringReference(strparam(L"There must be a digit following the optional exponent sign.")));
      }
    }else{
      success = false;
      AddStringRef(errorMessages, CreateStringReference(strparam(L"There must be a digit following optional the exponent sign.")));
    }
  }else{
    success = false;
    AddStringRef(errorMessages, CreateStringReference(strparam(L"There must be a sign or a digit following e or E.")));
  }

  return success;
}
_Bool IsJSONNumberCharacter(wchar_t c){
  wchar_t *numericCharacters;
  size_t numericCharactersLength;
  _Bool found;
  double i;

  numericCharacters = (wchar_t *)(L"0123456789.-+eE");
  numericCharactersLength = wcslen(numericCharacters);

  found = false;

  for(i = 0.0; i < numericCharactersLength; i = i + 1.0){
    if(numericCharacters[(int)(i)] == c){
      found = true;
    }
  }

  return found;
}
_Bool GetJSONPrimitiveName(wchar_t *string, size_t stringLength, double start, StringArrayReference *errorMessages, wchar_t *primitive, size_t primitiveLength, StringReference *tokenReference){
  wchar_t c, p;
  _Bool done, success;
  double i;
  wchar_t *str, *token;
  size_t strLength, tokenLength;

  done = false;
  success = true;

  token = (wchar_t *)(L"");
  tokenLength = wcslen(token);

  for(i = start; i < stringLength && ((i - start) < primitiveLength) &&  !done ; i = i + 1.0){
    c = string[(int)(i)];
    p = primitive[(int)(i - start)];
    if(c == p){
      /* OK */
      if((i + 1.0 - start) == primitiveLength){
        done = true;
      }
    }else{
      str = (wchar_t *)(L"");
      strLength = wcslen(str);
      str = strConcatenateString(&strLength, str, strLength, strparam(L"Primitive invalid: "));
      str = strAppendCharacter(&strLength, str, strLength, c);
      str = strAppendString(&strLength, str, strLength, strparam(L" vs "));
      str = strAppendCharacter(&strLength, str, strLength, p);

      AddStringRef(errorMessages, CreateStringReference(str, strLength));
      done = true;
      success = false;
    }
  }

  if(done){
    if(StringsEqual(primitive, primitiveLength, strparam(L"false"))){
      token = (wchar_t *)(L"false");
      tokenLength = wcslen(token);
    }
    if(StringsEqual(primitive, primitiveLength, strparam(L"true"))){
      token = (wchar_t *)(L"true");
      tokenLength = wcslen(token);
    }
    if(StringsEqual(primitive, primitiveLength, strparam(L"null"))){
      token = (wchar_t *)(L"null");
      tokenLength = wcslen(token);
    }
  }else{
    AddStringRef(errorMessages, CreateStringReference(strparam(L"Primitive invalid")));
    success = false;
  }

  tokenReference->string = token;
  tokenReference->stringLength = tokenLength;

  return success;
}
_Bool GetJSONString(wchar_t *json, size_t jsonLength, double start, StringReference *tokenReference, NumberReference *stringLengthReference, StringArrayReference *errorMessages){
  _Bool success, done;
  wchar_t *string, *hex;
  size_t stringLength, hexLength;
  NumberReference *characterCount, *hexReference;
  double i, l, c;
  StringReference *errorMessage;

  characterCount = CreateNumberReference(0.0);
  hex = CreateString(&hexLength, 4.0, '0');
  hexReference = (NumberReference *)malloc(sizeof(NumberReference));
  //errorMessage = (StringReference *)malloc(sizeof(StringReference));

  success = IsValidJSONStringInJSON(json, jsonLength, start, characterCount, stringLengthReference, errorMessages);

  if(success){
    l = characterCount->numberValue;
    string = (wchar_t*)malloc(sizeof(wchar_t) * (l));
    stringLength = l;

    c = 0.0;
    string[(int)(c)] = '\"';
    c = c + 1.0;

    done = false;
    for(i = start + 1.0;  !done ; i = i + 1.0){
      if(json[(int)(i)] == '\\'){
        i = i + 1.0;
        if(json[(int)(i)] == '\"' || json[(int)(i)] == '\\' || json[(int)(i)] == '/'){
          string[(int)(c)] = json[(int)(i)];
          c = c + 1.0;
        }else if(json[(int)(i)] == 'b'){
          string[(int)(c)] = 8.0;
          c = c + 1.0;
        }else if(json[(int)(i)] == 'f'){
          string[(int)(c)] = 12.0;
          c = c + 1.0;
        }else if(json[(int)(i)] == 'n'){
          string[(int)(c)] = 10.0;
          c = c + 1.0;
        }else if(json[(int)(i)] == 'r'){
          string[(int)(c)] = 13.0;
          c = c + 1.0;
        }else if(json[(int)(i)] == 't'){
          string[(int)(c)] = 9.0;
          c = c + 1.0;
        }else if(json[(int)(i)] == 'u'){
          i = i + 1.0;
          hex[0] = charToUpperCase(json[(int)(i + 0.0)]);
          hex[1] = charToUpperCase(json[(int)(i + 1.0)]);
          hex[2] = charToUpperCase(json[(int)(i + 2.0)]);
          hex[3] = charToUpperCase(json[(int)(i + 3.0)]);
          nCreateNumberFromStringWithCheck(hex, hexLength, 16.0, hexReference, errorMessage);
          string[(int)(c)] = hexReference->numberValue;
          i = i + 3.0;
          c = c + 1.0;
        }
      }else if(json[(int)(i)] == '\"'){
        string[(int)(c)] = json[(int)(i)];
        c = c + 1.0;
        done = true;
      }else{
        string[(int)(c)] = json[(int)(i)];
        c = c + 1.0;
      }
    }

    tokenReference->string = string;
    tokenReference->stringLength = stringLength;
    success = true;
  }else{
    AddStringRef(errorMessages, CreateStringReference(strparam(L"End of string was not found.")));
    success = false;
  }
  free(hex);
  free(hexReference);
  free(characterCount);

  return success;
}
_Bool IsValidJSONString(wchar_t *jsonString, size_t jsonStringLength, StringArrayReference *errorMessages){
  _Bool valid;
  NumberReference *numberReference, *stringLength;

  numberReference = (NumberReference *)malloc(sizeof(NumberReference));
  stringLength = (NumberReference *)malloc(sizeof(NumberReference));

  valid = IsValidJSONStringInJSON(jsonString, jsonStringLength, 0.0, numberReference, stringLength, errorMessages);

  return valid;
}
_Bool IsValidJSONStringInJSON(wchar_t *json, size_t jsonLength, double start, NumberReference *characterCount, NumberReference *stringLengthReference, StringArrayReference *errorMessages){
  _Bool success, done;
  double i, j;
  wchar_t c;

  success = true;
  done = false;

  characterCount->numberValue = 1.0;

  for(i = start + 1.0; i < jsonLength &&  !done  && success; i = i + 1.0){
    if( !IsJSONIllegalControllCharacter(json[(int)(i)]) ){
      if(json[(int)(i)] == '\\'){
        i = i + 1.0;
        if(i < jsonLength){
          if(json[(int)(i)] == '\"' || json[(int)(i)] == '\\' || json[(int)(i)] == '/' || json[(int)(i)] == 'b' || json[(int)(i)] == 'f' || json[(int)(i)] == 'n' || json[(int)(i)] == 'r' || json[(int)(i)] == 't'){
            characterCount->numberValue = characterCount->numberValue + 1.0;
          }else if(json[(int)(i)] == 'u'){
            if(i + 4.0 < jsonLength){
              for(j = 0.0; j < 4.0 && success; j = j + 1.0){
                c = json[(int)(i + j + 1.0)];
                if(nCharacterIsNumberCharacterInBase(c, 16.0) || c == 'a' || c == 'b' || c == 'c' || c == 'd' || c == 'e' || c == 'f'){
                }else{
                  success = false;
                  AddStringRef(errorMessages, CreateStringReference(strparam(L"\\u must be followed by four hexadecimal digits.")));
                }
              }
              characterCount->numberValue = characterCount->numberValue + 1.0;
              i = i + 4.0;
            }else{
              success = false;
              AddStringRef(errorMessages, CreateStringReference(strparam(L"\\u must be followed by four characters.")));
            }
          }else{
            success = false;
            AddStringRef(errorMessages, CreateStringReference(strparam(L"Escaped character invalid.")));
          }
        }else{
          success = false;
          AddStringRef(errorMessages, CreateStringReference(strparam(L"There must be at least two character after string escape.")));
        }
      }else if(json[(int)(i)] == '\"'){
        characterCount->numberValue = characterCount->numberValue + 1.0;
        done = true;
      }else{
        characterCount->numberValue = characterCount->numberValue + 1.0;
      }
    }else{
      success = false;
      AddStringRef(errorMessages, CreateStringReference(strparam(L"Unicode code points 0-31 not allowed in JSON string.")));
    }
  }

  if(done){
    stringLengthReference->numberValue = i - start;
  }else{
    success = false;
    AddStringRef(errorMessages, CreateStringReference(strparam(L"String must end with \".")));
  }

  return success;
}
_Bool IsJSONIllegalControllCharacter(wchar_t c){
  double cnr;
  _Bool isControll;

  cnr = c;

  if(cnr >= 0.0 && cnr < 32.0){
    isControll = true;
  }else{
    isControll = false;
  }

  return isControll;
}
Element **AddElement(size_t *returnArrayLength, Element **list, size_t listLength, Element *a){
  Element **newlist;
  size_t newlistLength;
  double i;

  newlist = (Element**)malloc(sizeof(Element) * listLength + 1.0);
  newlistLength = listLength + 1.0;

  for(i = 0.0; i < listLength; i = i + 1.0){
    newlist[(int)(i)] = list[(int)(i)];
  }
  newlist[(int)(listLength)] = a;

  free(list);

  *returnArrayLength = newlistLength;
  return newlist;
}
void AddElementRef(ElementArrayReference *list, Element *i){
  list->array = AddElement(&list->arrayLength, list->array, list->arrayLength, i);
}
Element **RemoveElement(size_t *returnArrayLength, Element **list, size_t listLength, double n){
  Element **newlist;
  size_t newlistLength;
  double i;

  newlist = (Element**)malloc(sizeof(Element) * listLength - 1.0);
  newlistLength = listLength - 1.0;

  for(i = 0.0; i < listLength; i = i + 1.0){
    if(i < n){
      newlist[(int)(i)] = list[(int)(i)];
    }
    if(i > n){
      newlist[(int)(i - 1.0)] = list[(int)(i)];
    }
  }

  free(list);

  *returnArrayLength = newlistLength;
  return newlist;
}
Element *GetElementRef(ElementArrayReference *list, double i){
  return list->array[(int)(i)];
}
void RemoveElementRef(ElementArrayReference *list, double i){
  list->array = RemoveElement(&list->arrayLength, list->array, list->arrayLength, i);
}
LinkedListElements *CreateLinkedListElements(){
  LinkedListElements *ll;

  ll = (LinkedListElements *)malloc(sizeof(LinkedListElements));
  ll->first = (LinkedListNodeElements *)malloc(sizeof(LinkedListNodeElements));
  ll->last = ll->first;
  ll->last->end = true;

  return ll;
}
void LinkedListAddElement(LinkedListElements *ll, Element *value){
  ll->last->end = false;
  ll->last->value = value;
  ll->last->next = (LinkedListNodeElements *)malloc(sizeof(LinkedListNodeElements));
  ll->last->next->end = true;
  ll->last = ll->last->next;
}
Element **LinkedListElementsToArray(size_t *returnArrayLength, LinkedListElements *ll){
  Element **array;
  size_t arrayLength;
  double length, i;
  LinkedListNodeElements *node;

  node = ll->first;

  length = LinkedListElementsLength(ll);

  array = (Element**)malloc(sizeof(Element) * length);
  arrayLength = length;

  for(i = 0.0; i < length; i = i + 1.0){
    array[(int)(i)] = node->value;
    node = node->next;
  }

  *returnArrayLength = arrayLength;
  return array;
}
double LinkedListElementsLength(LinkedListElements *ll){
  double l;
  LinkedListNodeElements *node;

  l = 0.0;
  node = ll->first;
  for(;  !node->end ; ){
    node = node->next;
    l = l + 1.0;
  }

  return l;
}
void FreeLinkedListElements(LinkedListElements *ll){
  LinkedListNodeElements *node, *prev;

  node = ll->first;

  for(;  !node->end ; ){
    prev = node;
    node = node->next;
    free(prev);
  }

  free(node);
  free(ll);
}
double ComputeJSONStringLength(Element *element){
  double result;

  result = 0.0;

  if(StringsEqual(element->type, element->typeLength, strparam(L"object"))){
    result = result + ComputeJSONObjectStringLength(element);
  }else if(StringsEqual(element->type, element->typeLength, strparam(L"string"))){
    result = JSONEscapedStringLength(element->string, element->stringLength) + 2.0;
  }else if(StringsEqual(element->type, element->typeLength, strparam(L"array"))){
    result = result + ComputeJSONArrayStringLength(element);
  }else if(StringsEqual(element->type, element->typeLength, strparam(L"number"))){
    result = result + ComputeJSONNumberStringLength(element);
  }else if(StringsEqual(element->type, element->typeLength, strparam(L"null"))){
    result = result + wcslen(L"null");
  }else if(StringsEqual(element->type, element->typeLength, strparam(L"boolean"))){
    result = result + ComputeJSONBooleanStringLength(element);
  }

  return result;
}
double ComputeJSONBooleanStringLength(Element *element){
  double result;

  if(element->booleanValue){
    result = wcslen(L"true");
  }else{
    result = wcslen(L"false");
  }

  return result;
}
double ComputeJSONNumberStringLength(Element *element){
  double length;
  wchar_t *a;
  size_t aLength;

  if(element->number != 0.0){
    if(fabs(element->number) >= pow(10.0, 15.0) || fabs(element->number) <= pow(10.0,  -15.0)){
      a = nCreateStringScientificNotationDecimalFromNumber(&aLength, element->number);
      length = aLength;
    }else{
      a = nCreateStringDecimalFromNumber(&aLength, element->number);
      length = aLength;
      free(a);
    }
  }else{
    length = 1.0;
  }

  return length;
}
double ComputeJSONArrayStringLength(Element *element){
  Element *arrayElement;
  double i;
  double length;

  length = 1.0;

  for(i = 0.0; i < element->arrayLength; i = i + 1.0){
    arrayElement = element->array[(int)(i)];

    length = length + ComputeJSONStringLength(arrayElement);

    if(i + 1.0 != element->arrayLength){
      length = length + 1.0;
    }
  }

  length = length + 1.0;

  return length;
}
double ComputeJSONObjectStringLength(Element *element){
  wchar_t *key;
  size_t keyLength;
  double i;
  StringArrayReference *keys;
  Element *objectElement;
  double length;

  length = 1.0;

  keys = GetStringElementMapKeySet(element->object);
  for(i = 0.0; i < keys->stringArrayLength; i = i + 1.0){
    key = keys->stringArray[(int)(i)]->string;
    keyLength = keys->stringArray[(int)(i)]->stringLength;
    objectElement = GetObjectValue(element->object, key, keyLength);

    length = length + 1.0;
    length = length + JSONEscapedStringLength(key, keyLength);
    length = length + 1.0;
    length = length + 1.0;

    length = length + ComputeJSONStringLength(objectElement);

    if(i + 1.0 != keys->stringArrayLength){
      length = length + 1.0;
    }
  }

  length = length + 1.0;

  return length;
}
Element *CreateStringElement(wchar_t *string, size_t stringLength){
  Element *element;
  element = (Element *)malloc(sizeof(Element));
  element->type = (wchar_t *)(L"string");
  element->typeLength = wcslen(element->type);
  element->string = string;
  element->stringLength = stringLength;
  return element;
}
Element *CreateBooleanElement(_Bool booleanValue){
  Element *element;
  element = (Element *)malloc(sizeof(Element));
  element->type = (wchar_t *)(L"boolean");
  element->typeLength = wcslen(element->type);
  element->booleanValue = booleanValue;
  return element;
}
Element *CreateNullElement(){
  Element *element;
  element = (Element *)malloc(sizeof(Element));
  element->type = (wchar_t *)(L"null");
  element->typeLength = wcslen(element->type);
  return element;
}
Element *CreateNumberElement(double number){
  Element *element;
  element = (Element *)malloc(sizeof(Element));
  element->type = (wchar_t *)(L"number");
  element->typeLength = wcslen(element->type);
  element->number = number;
  return element;
}
Element *CreateArrayElement(double length){
  Element *element;
  element = (Element *)malloc(sizeof(Element));
  element->type = (wchar_t *)(L"array");
  element->typeLength = wcslen(element->type);
  element->array = (Element**)malloc(sizeof(Element) * length);
  element->arrayLength = length;
  return element;
}
Element *CreateObjectElement(double length){
  Element *element;
  element = (Element *)malloc(sizeof(Element));
  element->type = (wchar_t *)(L"object");
  element->typeLength = wcslen(element->type);
  element->object = (StringElementMap *)malloc(sizeof(StringElementMap));
  element->object->stringListRef = CreateStringArrayReferenceLengthValue(length, strparam(L""));
  element->object->elementListRef = (ElementArrayReference *)malloc(sizeof(ElementArrayReference));
  element->object->elementListRef->array = (Element**)malloc(sizeof(Element) * length);
  element->object->elementListRef->arrayLength = length;
  return element;
}
void DeleteElement(Element *element){
  if(StringsEqual(element->type, element->typeLength, strparam(L"object"))){
    DeleteObject(element);
  }else if(StringsEqual(element->type, element->typeLength, strparam(L"string"))){
    free(element);
  }else if(StringsEqual(element->type, element->typeLength, strparam(L"array"))){
    DeleteArray(element);
  }else if(StringsEqual(element->type, element->typeLength, strparam(L"number"))){
    free(element);
  }else if(StringsEqual(element->type, element->typeLength, strparam(L"null"))){
    free(element);
  }else if(StringsEqual(element->type, element->typeLength, strparam(L"boolean"))){
    free(element);
  }else{
  }
}
void DeleteObject(Element *element){
  StringArrayReference *keys;
  double i;
  wchar_t *key;
  size_t keyLength;
  Element *objectElement;

  keys = GetStringElementMapKeySet(element->object);
  for(i = 0.0; i < keys->stringArrayLength; i = i + 1.0){
    key = keys->stringArray[(int)(i)]->string;
    keyLength = keys->stringArray[(int)(i)]->stringLength;
    objectElement = GetObjectValue(element->object, key, keyLength);
    DeleteElement(objectElement);
  }
  //for(i = 0.0; i < element->object->elementListRef->arrayLength; i = i + 1.0){
    //if (element->object->elementListRef->array[(int)(i)]->stringLength > 0.0)
        //free(element->object->elementListRef->array[(int)(i)]->string);
    //free(element->object->elementListRef->array[(int)(i)]);
    //if ((element->object->elementListRef->array[(int)(i)])&&(element->object->elementListRef->array[(int)(i)]->string))
      //free(element->object->elementListRef->array[(int)(i)]->string);
      //printf("Del: %p, %ls\n", element->object->elementListRef->array[(int)(i)]->string, element->object->elementListRef->array[(int)(i)]->string);
  //}
  for(i = 0.0; i < element->object->stringListRef->stringArrayLength; i = i + 1.0){
    //printf("%p, %ls\n", element->object->stringListRef->stringArray[(int)(i)]->string, element->object->stringListRef->stringArray[(int)(i)]->string);
    //if (element->object->stringListRef->stringArray[(int)(i)]->stringLength > 0.0)
    //  free(element->object->stringListRef->stringArray[(int)(i)]->string);
    free(element->object->stringListRef->stringArray[(int)(i)]);
  }
  free(element->object->stringListRef->stringArray);
  free(element->object->stringListRef);
  free(element->object->elementListRef->array);
  free(element->object->elementListRef);
  free(element->object);
  free(element);
}
void DeleteArray(Element *element){
  double i;
  Element *arrayElement;

  for(i = 0.0; i < element->arrayLength; i = i + 1.0){
    arrayElement = element->array[(int)(i)];
    DeleteElement(arrayElement);
  }
  free(element->array);
  free(element);
}
wchar_t *WriteJSON(size_t *returnArrayLength, Element *element){
  wchar_t *result;
  size_t resultLength;
  double length;
  NumberReference *index;

  length = ComputeJSONStringLength(element);
  result = (wchar_t*)malloc(sizeof(wchar_t) * (length+1));
  resultLength = length;
  index = CreateNumberReference(0.0);

  if(StringsEqual(element->type, element->typeLength, strparam(L"object"))){
    WriteObject(element, result, resultLength, index);
  }else if(StringsEqual(element->type, element->typeLength, strparam(L"string"))){
    WriteString(element, result, resultLength, index);
  }else if(StringsEqual(element->type, element->typeLength, strparam(L"array"))){
    WriteArray(element, result, resultLength, index);
  }else if(StringsEqual(element->type, element->typeLength, strparam(L"number"))){
    WriteNumber(element, result, resultLength, index);
  }else if(StringsEqual(element->type, element->typeLength, strparam(L"null"))){
    strWriteStringToStingStream(result, resultLength, index, strparam(L"null"));
  }else if(StringsEqual(element->type, element->typeLength, strparam(L"boolean"))){
    WriteBooleanValue(element, result, resultLength, index);
  }

  free(index);
  *returnArrayLength = resultLength;
  result[resultLength] = '\0';
  return result;
}
void WriteBooleanValue(Element *element, wchar_t *result, size_t resultLength, NumberReference *index){
  if(element->booleanValue){
    strWriteStringToStingStream(result, resultLength, index, strparam(L"true"));
  }else{
    strWriteStringToStingStream(result, resultLength, index, strparam(L"false"));
  }
}
void WriteNumber(Element *element, wchar_t *result, size_t resultLength, NumberReference *index){
  wchar_t *numberString;
  size_t numberStringLength;

  if(element->number != 0.0){
    if(fabs(element->number) >= pow(10.0, 15.0) || fabs(element->number) <= pow(10.0,  -15.0)){
      numberString = nCreateStringScientificNotationDecimalFromNumber(&numberStringLength, element->number);
    }else{
      numberString = nCreateStringDecimalFromNumber(&numberStringLength, element->number);
    }
  }else{
    numberString = nCreateStringDecimalFromNumber(&numberStringLength, element->number);
  }

  strWriteStringToStingStream(result, resultLength, index, numberString, numberStringLength);
  free(numberString);
}
void WriteArray(Element *element, wchar_t *result, size_t resultLength, NumberReference *index){
  wchar_t *s;
  size_t sLength;
  Element *arrayElement;
  double i;

  strWriteStringToStingStream(result, resultLength, index, strparam(L"["));

  for(i = 0.0; i < element->arrayLength; i = i + 1.0){
    arrayElement = element->array[(int)(i)];

    s = WriteJSON(&sLength, arrayElement);
    strWriteStringToStingStream(result, resultLength, index, s, sLength);
    free(s);

    if(i + 1.0 != element->arrayLength){
      strWriteStringToStingStream(result, resultLength, index, strparam(L","));
    }
  }

  strWriteStringToStingStream(result, resultLength, index, strparam(L"]"));
}
void WriteString(Element *element, wchar_t *result, size_t resultLength, NumberReference *index){
  strWriteStringToStingStream(result, resultLength, index, strparam(L"\""));
  wchar_t *string = JSONEscapeString(&element->stringLength, element->string, element->stringLength);
  strWriteStringToStingStream(result, resultLength, index, string, element->stringLength);
  strWriteStringToStingStream(result, resultLength, index, strparam(L"\""));
  free(string);
}
wchar_t *JSONEscapeString(size_t *returnArrayLength, wchar_t *string, size_t stringLength){
  double i, length;
  NumberReference *index, *lettersReference;
  wchar_t *ns, *escaped;
  size_t nsLength, escapedLength;

  length = JSONEscapedStringLength(string, stringLength);

  ns = (wchar_t*)malloc(sizeof(wchar_t) * (length));
  nsLength = length;
  index = CreateNumberReference(0.0);
  lettersReference = CreateNumberReference(0.0);

  for(i = 0.0; i < stringLength; i = i + 1.0){
    if(JSONMustBeEscaped(string[(int)(i)], lettersReference)){
      escaped = JSONEscapeCharacter(&escapedLength, string[(int)(i)]);
      strWriteStringToStingStream(ns, nsLength, index, escaped, escapedLength);
    }else{
      strWriteCharacterToStingStream(ns, nsLength, index, string[(int)(i)]);
    }
  }

  free(index);
  free(lettersReference);
  *returnArrayLength = nsLength;
  return ns;
}
double JSONEscapedStringLength(wchar_t *string, size_t stringLength){
  NumberReference *lettersReference;
  double i, length;

  lettersReference = CreateNumberReference(0.0);
  length = 0.0;

  for(i = 0.0; i < stringLength; i = i + 1.0){
    if(JSONMustBeEscaped(string[(int)(i)], lettersReference)){
      length = length + lettersReference->numberValue;
    }else{
      length = length + 1.0;
    }
  }
  free(lettersReference);
  return length;
}
wchar_t *JSONEscapeCharacter(size_t *returnArrayLength, wchar_t c){
  double code;
  wchar_t *escaped;
  size_t escapedLength;
  StringReference *hexNumber;
  double q, rs, s, b, f, n, r, t;

  code = c;

  q = 34.0;
  rs = 92.0;
  s = 47.0;
  b = 8.0;
  f = 12.0;
  n = 10.0;
  r = 13.0;
  t = 9.0;

  hexNumber = (StringReference *)malloc(sizeof(StringReference));

  if(code == q){
    escaped = (wchar_t*)malloc(sizeof(wchar_t) * (2.0));
    escapedLength = 2.0;
    escaped[0] = '\\';
    escaped[1] = '\"';
  }else if(code == rs){
    escaped = (wchar_t*)malloc(sizeof(wchar_t) * (2.0));
    escapedLength = 2.0;
    escaped[0] = '\\';
    escaped[1] = '\\';
  }else if(code == s){
    escaped = (wchar_t*)malloc(sizeof(wchar_t) * (2.0));
    escapedLength = 2.0;
    escaped[0] = '\\';
    escaped[1] = '/';
  }else if(code == b){
    escaped = (wchar_t*)malloc(sizeof(wchar_t) * (2.0));
    escapedLength = 2.0;
    escaped[0] = '\\';
    escaped[1] = 'b';
  }else if(code == f){
    escaped = (wchar_t*)malloc(sizeof(wchar_t) * (2.0));
    escapedLength = 2.0;
    escaped[0] = '\\';
    escaped[1] = 'f';
  }else if(code == n){
    escaped = (wchar_t*)malloc(sizeof(wchar_t) * (2.0));
    escapedLength = 2.0;
    escaped[0] = '\\';
    escaped[1] = 'n';
  }else if(code == r){
    escaped = (wchar_t*)malloc(sizeof(wchar_t) * (2.0));
    escapedLength = 2.0;
    escaped[0] = '\\';
    escaped[1] = 'r';
  }else if(code == t){
    escaped = (wchar_t*)malloc(sizeof(wchar_t) * (2.0));
    escapedLength = 2.0;
    escaped[0] = '\\';
    escaped[1] = 't';
  }else if(code >= 0.0 && code <= 31.0){
    escaped = (wchar_t*)malloc(sizeof(wchar_t) * (6.0));
    escapedLength = 6.0;
    escaped[0] = '\\';
    escaped[1] = 'u';
    escaped[2] = '0';
    escaped[3] = '0';

    nCreateStringFromNumberWithCheck(code, 16.0, hexNumber);

    if(hexNumber->stringLength == 1.0){
      escaped[4] = '0';
      escaped[5] = hexNumber->string[0];
    }else if(hexNumber->stringLength == 2.0){
      escaped[4] = hexNumber->string[0];
      escaped[5] = hexNumber->string[1];
    }
  }else{
    escaped = (wchar_t*)malloc(sizeof(wchar_t) * (1.0));
    escapedLength = 1.0;
    escaped[0] = c;
  }

  *returnArrayLength = escapedLength;
  return escaped;
}
_Bool JSONMustBeEscaped(wchar_t c, NumberReference *letters){
  double code;
  _Bool mustBeEscaped;
  double q, rs, s, b, f, n, r, t;

  code = c;
  mustBeEscaped = false;

  q = 34.0;
  rs = 92.0;
  s = 47.0;
  b = 8.0;
  f = 12.0;
  n = 10.0;
  r = 13.0;
  t = 9.0;

  if(code == q || code == rs || code == s || code == b || code == f || code == n || code == r || code == t){
    mustBeEscaped = true;
    letters->numberValue = 2.0;
  }else if(code >= 0.0 && code <= 31.0){
    mustBeEscaped = true;
    letters->numberValue = 6.0;
  }else if(code >= pow(2.0, 16.0)){
    mustBeEscaped = true;
    letters->numberValue = 6.0;
  }

  return mustBeEscaped;
}
void WriteObject(Element *element, wchar_t *result, size_t resultLength, NumberReference *index){
  wchar_t *s, *key;
  size_t sLength, keyLength;
  double i;
  StringArrayReference *keys;
  Element *objectElement;

  strWriteStringToStingStream(result, resultLength, index, strparam(L"{"));

  keys = GetStringElementMapKeySet(element->object);
  for(i = 0.0; i < keys->stringArrayLength; i = i + 1.0){
    key = keys->stringArray[(int)(i)]->string;
    keyLength = keys->stringArray[(int)(i)]->stringLength;
    key = JSONEscapeString(&keyLength, key, keyLength);
    objectElement = GetObjectValue(element->object, key, keyLength);

    strWriteStringToStingStream(result, resultLength, index, strparam(L"\""));
    strWriteStringToStingStream(result, resultLength, index, key, keyLength);
    strWriteStringToStingStream(result, resultLength, index, strparam(L"\""));
    strWriteStringToStingStream(result, resultLength, index, strparam(L":"));
    free(key);

    s = WriteJSON(&sLength, objectElement);
    strWriteStringToStingStream(result, resultLength, index, s, sLength);
    free(s);

    if(i + 1.0 != keys->stringArrayLength){
      strWriteStringToStingStream(result, resultLength, index, strparam(L","));
    }
  }

  strWriteStringToStingStream(result, resultLength, index, strparam(L"}"));
}
_Bool ReadJSON(wchar_t *string, size_t stringLength, ElementReference *elementReference, StringArrayReference *errorMessages){
  StringArrayReference *tokenArrayReference;
  _Bool success;
  int i;

  /* Tokenize. */
  tokenArrayReference = (StringArrayReference *)malloc(sizeof(StringArrayReference));
  success = JSONTokenize(string, stringLength, tokenArrayReference, errorMessages);

  if(success){
    /* Parse. */
    success = GetJSONValue(tokenArrayReference->stringArray, tokenArrayReference->stringArrayLength, elementReference, errorMessages);
  }
  for(i = 0.0; i < tokenArrayReference->stringArrayLength; i = i + 1.0){
    if ((tokenArrayReference->stringArray[(int)(i)]->string)&&(tokenArrayReference->stringArray[(int)(i)]->stringLength > 1.0)) {
      //printf("%p, %ls\n",tokenArrayReference->stringArray[(int)(i)]->string, tokenArrayReference->stringArray[(int)(i)]->string);
      if(StringsEqual(tokenArrayReference->stringArray[(int)(i)]->string, tokenArrayReference->stringArray[(int)(i)]->stringLength, strparam(L"false"))){
      } else if(StringsEqual(tokenArrayReference->stringArray[(int)(i)]->string, tokenArrayReference->stringArray[(int)(i)]->stringLength, strparam(L"true"))){
      } else if(StringsEqual(tokenArrayReference->stringArray[(int)(i)]->string, tokenArrayReference->stringArray[(int)(i)]->stringLength, strparam(L"null"))){
      } else if(StringsEqual(tokenArrayReference->stringArray[(int)(i)]->string, tokenArrayReference->stringArray[(int)(i)]->stringLength, strparam(L"<end>"))){
      } else {
        free(tokenArrayReference->stringArray[(int)(i)]->string);
      }
    }
    free(tokenArrayReference->stringArray[(int)(i)]);
  }
  free(tokenArrayReference->stringArray);
  //FreeStringArrayReference(tokenArrayReference);
  free(tokenArrayReference);
  return success;
}
_Bool GetJSONValue(StringReference **tokens, size_t tokensLength, ElementReference *elementReference, StringArrayReference *errorMessages){
  NumberReference *i;
  _Bool success;

  i = CreateNumberReference(0.0);
  success = GetJSONValueRecursive(tokens, tokensLength, i, 0.0, elementReference, errorMessages);
  free(i);

  return success;
}
_Bool GetJSONValueRecursive(StringReference **tokens, size_t tokensLength, NumberReference *i, double depth, ElementReference *elementReference, StringArrayReference *errorMessages){
  wchar_t *str, *substr, *token;
  size_t strLength, substrLength, tokenLength;
  double stringToDecimalResult;
  _Bool success;

  success = true;
  token = tokens[(int)(i->numberValue)]->string;
  tokenLength = tokens[(int)(i->numberValue)]->stringLength;

  if(StringsEqual(token, tokenLength, strparam(L"{"))){
    success = GetJSONObject(tokens, tokensLength, i, depth + 1.0, elementReference, errorMessages);
  }else if(StringsEqual(token, tokenLength, strparam(L"["))){
    success = GetJSONArray(tokens, tokensLength, i, depth + 1.0, elementReference, errorMessages);
  }else if(StringsEqual(token, tokenLength, strparam(L"true"))){
    elementReference->element = CreateBooleanElement(true);
    i->numberValue = i->numberValue + 1.0;
  }else if(StringsEqual(token, tokenLength, strparam(L"false"))){
    elementReference->element = CreateBooleanElement(false);
    i->numberValue = i->numberValue + 1.0;
  }else if(StringsEqual(token, tokenLength, strparam(L"null"))){
    elementReference->element = CreateNullElement();
    i->numberValue = i->numberValue + 1.0;
  }else if(charIsNumber(token[0]) || token[0] == '-'){
    stringToDecimalResult = nCreateNumberFromDecimalString(token, tokenLength);
    elementReference->element = CreateNumberElement(stringToDecimalResult);
    i->numberValue = i->numberValue + 1.0;
  }else if(token[0] == '\"'){
    substr = strSubstring(&substrLength, token, tokenLength, 1.0, tokenLength - 1.0);
    elementReference->element = CreateStringElement(substr, substrLength);
    i->numberValue = i->numberValue + 1.0;
    //free(substr);
  }else{
    str = (wchar_t *)(L"");
    strLength = wcslen(str);
    str = strConcatenateString(&strLength, str, strLength, strparam(L"Invalid token first in value: "));
    str = strAppendString(&strLength, str, strLength, token, tokenLength);
    AddStringRef(errorMessages, CreateStringReference(str, strLength));
    success = false;
  }

  if(success && depth == 0.0){
    if(StringsEqual(tokens[(int)(i->numberValue)]->string, tokens[(int)(i->numberValue)]->stringLength, strparam(L"<end>"))){
    }else{
      AddStringRef(errorMessages, CreateStringReference(strparam(L"The outer value cannot have any tokens following it.")));
      success = false;
    }
  }

  return success;
}
_Bool GetJSONObject(StringReference **tokens, size_t tokensLength, NumberReference *i, double depth, ElementReference *elementReference, StringArrayReference *errorMessages){
  Element *element, *value;
  _Bool done, success;
  wchar_t *key, *colon, *comma, *closeCurly;
  size_t keyLength, colonLength, commaLength, closeCurlyLength;
  wchar_t *keystring, *str;
  size_t keystringLength, strLength;
  ElementReference *valueReference;
  LinkedListElements *values;
  LinkedListStrings *keys;

  keys = CreateLinkedListString();
  values = CreateLinkedListElements();
  element = CreateObjectElement(0.0);
  valueReference = (ElementReference *)malloc(sizeof(ElementReference));
  success = true;
  i->numberValue = i->numberValue + 1.0;

  if( !StringsEqual(tokens[(int)(i->numberValue)]->string, tokens[(int)(i->numberValue)]->stringLength, strparam(L"}")) ){
    done = false;

    for(;  !done  && success; ){
      key = tokens[(int)(i->numberValue)]->string;
      keyLength = tokens[(int)(i->numberValue)]->stringLength;

      if(key[0] == '\"'){
        i->numberValue = i->numberValue + 1.0;
        colon = tokens[(int)(i->numberValue)]->string;
        colonLength = tokens[(int)(i->numberValue)]->stringLength;
        if(StringsEqual(colon, colonLength, strparam(L":"))){
          i->numberValue = i->numberValue + 1.0;
          success = GetJSONValueRecursive(tokens, tokensLength, i, depth, valueReference, errorMessages);
          if(success){
            keystring = strSubstring(&keystringLength, key, keyLength, 1.0, keyLength - 1.0);
            value = valueReference->element;
            LinkedListAddString(keys, keystring, keystringLength);
            LinkedListAddElement(values, value);
       //if (value->string)
       //printf("VAL:%p, %ls\n", value->string, value->string);

            //free(keystring);

            comma = tokens[(int)(i->numberValue)]->string;
            commaLength = tokens[(int)(i->numberValue)]->stringLength;
            if(StringsEqual(comma, commaLength, strparam(L","))){
              /* OK */
              i->numberValue = i->numberValue + 1.0;
            }else{
              done = true;
            }
          }
        }else{
          str = (wchar_t *)(L"");
          strLength = wcslen(str);
          str = strConcatenateString(&strLength, str, strLength, strparam(L"Expected colon after key in object: "));
          str = strAppendString(&strLength, str, strLength, colon, colonLength);
          AddStringRef(errorMessages, CreateStringReference(str, strLength));

          success = false;
          done = true;
        }
      }else{
        AddStringRef(errorMessages, CreateStringReference(strparam(L"Expected string as key in object.")));

        done = true;
        success = false;
      }
    }
  }

  if(success){
    closeCurly = tokens[(int)(i->numberValue)]->string;
    closeCurlyLength = tokens[(int)(i->numberValue)]->stringLength;

    if(StringsEqual(closeCurly, closeCurlyLength, strparam(L"}"))){
      /* OK */
      free(element->object->stringListRef->stringArray);
      free(element->object->elementListRef->array);
      element->object->stringListRef->stringArray = LinkedListStringsToArray(&element->object->stringListRef->stringArrayLength, keys);
      element->object->elementListRef->array = LinkedListElementsToArray(&element->object->elementListRef->arrayLength, values);
      elementReference->element = element;
      i->numberValue = i->numberValue + 1.0;
    }else{
      AddStringRef(errorMessages, CreateStringReference(strparam(L"Expected close curly brackets at end of object value.")));
      success = false;
    }
  }

  FreeLinkedListString(keys);
  FreeLinkedListElements(values);
  free(valueReference);

  return success;
}
_Bool GetJSONArray(StringReference **tokens, size_t tokensLength, NumberReference *i, double depth, ElementReference *elementReference, StringArrayReference *errorMessages){
  Element *element, *value;
  wchar_t *nextToken, *comma;
  size_t nextTokenLength, commaLength;
  _Bool done, success;
  ElementReference *valueReference;
  LinkedListElements *elements;

  elements = CreateLinkedListElements();
  i->numberValue = i->numberValue + 1.0;

  valueReference = (ElementReference *)malloc(sizeof(ElementReference));
  success = true;
  element = CreateArrayElement(0.0);

  nextToken = tokens[(int)(i->numberValue)]->string;
  nextTokenLength = tokens[(int)(i->numberValue)]->stringLength;

  if( !StringsEqual(nextToken, nextTokenLength, strparam(L"]")) ){
    done = false;
    for(;  !done  && success; ){
      success = GetJSONValueRecursive(tokens, tokensLength, i, depth, valueReference, errorMessages);

      if(success){
        value = valueReference->element;
        LinkedListAddElement(elements, value);

        comma = tokens[(int)(i->numberValue)]->string;
        commaLength = tokens[(int)(i->numberValue)]->stringLength;

        if(StringsEqual(comma, commaLength, strparam(L","))){
          /* OK */
          i->numberValue = i->numberValue + 1.0;
        }else{
          done = true;
        }
      }
    }
  }

  nextToken = tokens[(int)(i->numberValue)]->string;
  nextTokenLength = tokens[(int)(i->numberValue)]->stringLength;
  if(StringsEqual(nextToken, nextTokenLength, strparam(L"]"))){
    /* OK */
    i->numberValue = i->numberValue + 1.0;
    free(element->array);
    element->array = LinkedListElementsToArray(&element->arrayLength, elements);
  }else{
    AddStringRef(errorMessages, CreateStringReference(strparam(L"Expected close square bracket at end of array.")));
    success = false;
  }

  elementReference->element = element;
  FreeLinkedListElements(elements);
  free(valueReference);

  return success;
}
StringArrayReference *GetStringElementMapKeySet(StringElementMap *stringElementMap){
  return stringElementMap->stringListRef;
}
Element *GetObjectValue(StringElementMap *stringElementMap, wchar_t *key, size_t keyLength){
  Element *result;
  double i;

  //result = (Element *)malloc(sizeof(Element));

  for(i = 0.0; i < GetStringElementMapNumberOfKeys(stringElementMap); i = i + 1.0){
    if(StringsEqual(stringElementMap->stringListRef->stringArray[(int)(i)]->string, stringElementMap->stringListRef->stringArray[(int)(i)]->stringLength, key, keyLength)){
      result = stringElementMap->elementListRef->array[(int)(i)];
    }
  }

  return result;
}
Element *GetObjectValueWithCheck(StringElementMap *stringElementMap, wchar_t *key, size_t keyLength, BooleanReference *foundReference){
  Element *result;
  double i;

  result = (Element *)malloc(sizeof(Element));

  foundReference->booleanValue = false;
  for(i = 0.0; i < GetStringElementMapNumberOfKeys(stringElementMap); i = i + 1.0){
    if(StringsEqual(stringElementMap->stringListRef->stringArray[(int)(i)]->string, stringElementMap->stringListRef->stringArray[(int)(i)]->stringLength, key, keyLength)){
      result = stringElementMap->elementListRef->array[(int)(i)];
      foundReference->booleanValue = true;
    }
  }

  return result;
}
void PutStringElementMap(StringElementMap *stringElementMap, wchar_t *keystring, size_t keystringLength, Element *value){
  AddStringRef(stringElementMap->stringListRef, CreateStringReference(keystring, keystringLength));
  AddElementRef(stringElementMap->elementListRef, value);
}
void SetStringElementMap(StringElementMap *stringElementMap, double index, wchar_t *keystring, size_t keystringLength, Element *value){
  stringElementMap->stringListRef->stringArray[(int)(index)]->string = keystring;
  stringElementMap->stringListRef->stringArray[(int)(index)]->stringLength = keystringLength;
  stringElementMap->elementListRef->array[(int)(index)] = value;
}
double GetStringElementMapNumberOfKeys(StringElementMap *stringElementMap){
  return stringElementMap->stringListRef->stringArrayLength;
}
_Bool JSONCompare(wchar_t *a, size_t aLength, wchar_t *b, size_t bLength, double epsilon, BooleanReference *equal, StringArrayReference *errorMessage){
  _Bool success;
  ElementReference *eaRef, *ebRef;
  Element *ea, *eb;

  eaRef = (ElementReference *)malloc(sizeof(ElementReference));
  ebRef = (ElementReference *)malloc(sizeof(ElementReference));

  success = ReadJSON(a, aLength, eaRef, errorMessage);

  if(success){
    ea = eaRef->element;

    success = ReadJSON(b, bLength, ebRef, errorMessage);

    if(success){
      eb = ebRef->element;

      equal->booleanValue = JSONCompareElements(ea, eb, epsilon);

      DeleteElement(eb);
    }

    DeleteElement(ea);
  }

  return success;
}
_Bool JSONCompareElements(Element *ea, Element *eb, double epsilon){
  _Bool equal;
  wchar_t *typeName;
  size_t typeNameLength;

  equal = StringsEqual(ea->type, ea->typeLength, eb->type, eb->typeLength);
        
  if(equal){
    typeName = ea->type;
    typeNameLength = ea->typeLength;
    if(StringsEqual(typeName, typeNameLength, strparam(L"object"))){
      equal = JSONCompareObjects(ea->object, eb->object, epsilon);
    }else if(StringsEqual(typeName, typeNameLength, strparam(L"string"))){
      equal = StringsEqual(ea->string, ea->stringLength, eb->string, eb->stringLength);
    }else if(StringsEqual(typeName, typeNameLength, strparam(L"array"))){
      equal = JSONCompareArrays(ea->array, ea->arrayLength, eb->array, eb->arrayLength, epsilon);
    }else if(StringsEqual(typeName, typeNameLength, strparam(L"number"))){
      equal = EpsilonCompare(ea->number, eb->number, epsilon);
    }else if(StringsEqual(typeName, typeNameLength, strparam(L"null"))){
      equal = true;
    }else if(StringsEqual(typeName, typeNameLength, strparam(L"boolean"))){
      equal = ea->booleanValue == eb->booleanValue;
    }
  }
        
  return equal;
}
_Bool JSONCompareArrays(Element **ea, size_t eaLength, Element **eb, size_t ebLength, double epsilon){
  _Bool equals;
  double i, length;

  equals = eaLength == ebLength;

  if(equals){
    length = eaLength;
    for(i = 0.0; i < length && equals; i = i + 1.0){
      equals = JSONCompareElements(ea[(int)(i)], eb[(int)(i)], epsilon);
    }
  }

  return equals;
}
_Bool JSONCompareObjects(StringElementMap *ea, StringElementMap *eb, double epsilon){
  _Bool equals;
  double akeys, bkeys, i;
  StringArrayReference *keys;
  wchar_t *key;
  size_t keyLength;
  BooleanReference *aFoundReference, *bFoundReference;
  Element *eaValue, *ebValue;

  aFoundReference = (BooleanReference *)malloc(sizeof(BooleanReference));
  bFoundReference = (BooleanReference *)malloc(sizeof(BooleanReference));

  akeys = GetStringElementMapNumberOfKeys(ea);
  bkeys = GetStringElementMapNumberOfKeys(eb);

  equals = akeys == bkeys;

  if(equals){
    keys = GetStringElementMapKeySet(ea);

    for(i = 0.0; i < keys->stringArrayLength && equals; i = i + 1.0){
      key = keys->stringArray[(int)(i)]->string;
      keyLength = keys->stringArray[(int)(i)]->stringLength;

      eaValue = GetObjectValueWithCheck(ea, key, keyLength, aFoundReference);
      ebValue = GetObjectValueWithCheck(eb, key, keyLength, bFoundReference);

      if(aFoundReference->booleanValue && bFoundReference->booleanValue){
        equals = JSONCompareElements(eaValue, ebValue, epsilon);
      }else{
        equals = false;
      }
    }
  }

  return equals;
}
void testEscaper(NumberReference *failures){
  wchar_t c;
  NumberReference *letters;
  _Bool mustBeEscaped;
  wchar_t *escaped;
  size_t escapedLength;

  letters = CreateNumberReference(0.0);

  c = 9.0;
  mustBeEscaped = JSONMustBeEscaped(c, letters);
  AssertTrue(mustBeEscaped, failures);
  AssertEquals(letters->numberValue, 2.0, failures);

  escaped = JSONEscapeCharacter(&escapedLength, c);
  AssertStringEquals(escaped, escapedLength, strparam(L"\\t"), failures);

  c = 0.0;
  mustBeEscaped = JSONMustBeEscaped(c, letters);
  AssertTrue(mustBeEscaped, failures);
  AssertEquals(letters->numberValue, 6.0, failures);

  escaped = JSONEscapeCharacter(&escapedLength, c);
  AssertStringEquals(escaped, escapedLength, strparam(L"\\u0000"), failures);
}
Example *mapTo(Element *root){
  Example *example;

  example = (Example *)malloc(sizeof(Example));
  example->a = GetObjectValue(root->object, strparam(L"a"))->string;
  example->aLength = GetObjectValue(root->object, strparam(L"a"))->stringLength;
  example->b = mapbTo(&example->bLength, GetObjectValue(root->object, strparam(L"b"))->array, GetObjectValue(root->object, strparam(L"b"))->arrayLength);
  example->x = mapXTo(GetObjectValue(root->object, strparam(L"x"))->object);

  return example;
}
X *mapXTo(StringElementMap *object){
  X *x;

  x = (X *)malloc(sizeof(X));

  if(StringsEqual(GetObjectValue(object, strparam(L"x1"))->type, GetObjectValue(object, strparam(L"x1"))->typeLength, strparam(L"null"))){
    x->x1IsNull = true;
    x->x1 = (wchar_t *)(L"");
    x->x1Length = wcslen(x->x1);
  }

  x->x2 = GetObjectValue(object, strparam(L"x2"))->booleanValue;
  x->x3 = GetObjectValue(object, strparam(L"x3"))->booleanValue;

  return x;
}
double *mapbTo(size_t *returnArrayLength, Element **array, size_t arrayLength){
  double *b;
  size_t bLength;
  double i;

  b = (double*)malloc(sizeof(double) * (arrayLength));
  bLength = arrayLength;

  for(i = 0.0; i < arrayLength; i = i + 1.0){
    b[(int)(i)] = array[(int)(i)]->number;
  }

  *returnArrayLength = bLength;
  return b;
}
void testWriter(NumberReference *failures){
  wchar_t *string;
  size_t stringLength;
  Element *root, *e;
  Example *example;
  wchar_t *result;
  size_t resultLength;
  NumberReference *numberIndex;

  root = createExampleJSON();

  string = WriteJSON(&stringLength, root);

  AssertEquals(stringLength, 66.0, failures);

  /* Does not work with Java Maps.. */
  example = mapTo(root);

  AssertStringEquals(strparam(L"hei"), example->a, example->aLength, failures);
  AssertTrue(example->x->x1IsNull, failures);
  AssertTrue(example->x->x2, failures);
  AssertFalse(example->x->x3, failures);
  AssertEquals(1.2, example->b[0], failures);
  AssertEquals(0.1, example->b[1], failures);
  AssertEquals(100.0, example->b[2], failures);

  DeleteElement(root);

  e = CreateNumberElement(0.0);
  result = (wchar_t*)malloc(sizeof(wchar_t) * (1.0));
  resultLength = 1.0;
  numberIndex = (NumberReference *)malloc(sizeof(NumberReference));
  WriteNumber(e, result, resultLength, numberIndex);

  AssertStringEquals(strparam(L"0"), result, resultLength, failures);
  free(result);
  free(numberIndex);
}
Element *createExampleJSON(){
  Element *root, *innerObject, *array;

  root = CreateObjectElement(3.0);

  innerObject = CreateObjectElement(3.0);

  SetStringElementMap(innerObject->object, 0.0, strparam(L"x1"), CreateNullElement());
  SetStringElementMap(innerObject->object, 1.0, strparam(L"x2"), CreateBooleanElement(true));
  SetStringElementMap(innerObject->object, 2.0, strparam(L"x3"), CreateBooleanElement(false));

  array = CreateArrayElement(3.0);
  array->array[0] = CreateNumberElement(1.2);
  array->array[1] = CreateNumberElement(0.1);
  array->array[2] = CreateNumberElement(100.0);

  SetStringElementMap(root->object, 0.0, strparam(L"a"), CreateStringElement(strparam(L"hei")));
  SetStringElementMap(root->object, 1.0, strparam(L"b"), array);
  SetStringElementMap(root->object, 2.0, strparam(L"x"), innerObject);

  return root;
}
void testWriterEscape(NumberReference *failures){
  wchar_t *string;
  size_t stringLength;
  Element *root;

  root = CreateStringElement(strparam(L"\t\n"));

  string = WriteJSON(&stringLength, root);

  AssertEquals(stringLength, 6.0, failures);

  AssertStringEquals(strparam(L"\"\\t\\n\""), string, stringLength, failures);

  DeleteElement(root);
}
void testWriter2(NumberReference *failures){
  Element *e;
  wchar_t *result, *json;
  size_t resultLength, jsonLength;
  ElementReference *obj;
  StringArrayReference *errorMessages;
  _Bool success;

  obj = (ElementReference *)malloc(sizeof(ElementReference));
  errorMessages = (StringArrayReference *)malloc(sizeof(StringArrayReference));
  json = (wchar_t *)(L"{\"test1\":0,\"test2\":0,\"test3\":0}");
  jsonLength = wcslen(json);
  success = ReadJSON(json, jsonLength, obj, errorMessages);

  AssertTrue(success, failures);

  if(success){
    e = obj->element;

    result = WriteJSON(&resultLength, e);

    AssertStringEquals(result, resultLength, json, jsonLength, failures);
  }
}
void testReader(NumberReference *failures){
  Element *json;
  wchar_t *string, *string2;
  size_t stringLength, string2Length;
  StringArrayReference *errorMessages;
  ElementReference *elementReference;
  _Bool success;

  json = createExampleJSON();
  string = WriteJSON(&stringLength, json);
  elementReference = (ElementReference *)malloc(sizeof(ElementReference));

  errorMessages = CreateStringArrayReferenceLengthValue(0.0, strparam(L""));

  success = ReadJSON(string, stringLength, elementReference, errorMessages);
  AssertTrue(success, failures);

  if(success){
    json = elementReference->element;
    string2 = WriteJSON(&string2Length, json);

    AssertEquals(stringLength, string2Length, failures);
  }
}
void test2(NumberReference *failures){
  wchar_t *string, *string2;
  size_t stringLength, string2Length;
  StringArrayReference *errorMessages;
  Element *json;
  ElementReference *elementReference;
  _Bool success;

  string = strConcatenateString(&stringLength, strparam(L"{"), strparam(L"\"name\":\"base64\","));
  string = strAppendString(&stringLength, string, stringLength, strparam(L"\"version\":\"0.1.0\","));
  string = strAppendString(&stringLength, string, stringLength, strparam(L"\"business namespace\":\"no.inductive.idea10.programs\","));
  string = strAppendString(&stringLength, string, stringLength, strparam(L"\"scientific namespace\":\"computerscience.algorithms.base64\","));
  string = strAppendString(&stringLength, string, stringLength, strparam(L"\"imports\":["));
  string = strAppendString(&stringLength, string, stringLength, strparam(L"],"));
  string = strAppendString(&stringLength, string, stringLength, strparam(L"\"imports2\":{"));
  string = strAppendString(&stringLength, string, stringLength, strparam(L"},"));
  string = strAppendString(&stringLength, string, stringLength, strparam(L"\"development imports\":["));
  string = strAppendString(&stringLength, string, stringLength, strparam(L"[\"\",\"no.inductive.idea10.programs\",\"arrays\",\"0.1.0\"]"));
  string = strAppendString(&stringLength, string, stringLength, strparam(L"]"));
  string = strAppendString(&stringLength, string, stringLength, strparam(L"}"));

  errorMessages = CreateStringArrayReferenceLengthValue(0.0, strparam(L""));
  elementReference = (ElementReference *)malloc(sizeof(ElementReference));
  success = ReadJSON(string, stringLength, elementReference, errorMessages);
  AssertTrue(success, failures);

  if(success){
    json = elementReference->element;

    string2 = WriteJSON(&string2Length, json);

    AssertEquals(stringLength, string2Length, failures);
  }
}
void testReaderExample(NumberReference *failures){
  wchar_t *json;
  size_t jsonLength;
  StringArrayReference *errorMessages;
  ElementReference *elementReference;
  StringReference *outputJSON;

  errorMessages = CreateStringArrayReferenceLengthValue(0.0, strparam(L""));
  elementReference = (ElementReference *)malloc(sizeof(ElementReference));
  outputJSON = CreateStringReference(strparam(L""));

  json = (wchar_t *)(L"{\"a\":\"hi\",\"b\":[1.2, 0.1, 100],\"x\":{\"x1\":null,\"x2\":true,\"x3\":false}}");
  jsonLength = wcslen(json);

  JSONExample(json, jsonLength, errorMessages, elementReference, outputJSON);
}
void JSONExample(wchar_t *json, size_t jsonLength, StringArrayReference *errorMessages, ElementReference *elementReference, StringReference *outputJSON){
  _Bool success;
  Element *element, *aElement;
  wchar_t *string;
  size_t stringLength;
  Element **array;
  size_t arrayLength;
  double x, y, z;

  /* The following JSON is in the string json:
           {
             "a": "hi",
             "b": [1.2, 0.1, 100],
             "x": {
               "x1": null,
               "x2": true,
               "x3": false
             }
           }
         */

  /* This functions reads the JSON */
  success = ReadJSON(json, jsonLength, elementReference, errorMessages);

  /* The return value 'success' is set to true of the parsing succeeds, */
  /* if not, errorMessages contains the reason. */
  if(success){
    /* We can now extract the data structure: */
    element = elementReference->element;

    /* The following is gets the value "hi" for key "a": */
    aElement = GetObjectValue(element->object, strparam(L"a"));
    string = aElement->string;
    stringLength = aElement->stringLength;

    /* The following is gets the array [1.2, 0.1, 100] for key "b": */
    aElement = GetObjectValue(element->object, strparam(L"b"));
    array = aElement->array;
    arrayLength = aElement->arrayLength;
    x = array[0]->number;
    y = array[1]->number;
    z = array[2]->number;

    /* This is how you write JSON: */
    outputJSON->string = WriteJSON(&outputJSON->stringLength, element);
  }else{
    /* There was a problem, so we cannot read our data structure. */
    /* Instead, we can check out the error message. */
    string = errorMessages->stringArray[0]->string;
    stringLength = errorMessages->stringArray[0]->stringLength;
  }
}
double test(){
  NumberReference *failures;

  failures = CreateNumberReference(0.0);

  testReader(failures);
  test2(failures);
  testWriter(failures);
  testWriterEscape(failures);
  testTokenizer1(failures);
  testReaderExample(failures);
  testEscaper(failures);
  testValidator(failures);
  testComparator(failures);
  testWriter2(failures);

  return failures->numberValue;
}
void testValidator(NumberReference *failures){
  wchar_t *a, *b, *c, *d;
  size_t aLength, bLength, cLength, dLength;
  StringArrayReference *errorMessages;

  errorMessages = CreateStringArrayReferenceLengthValue(0.0, strparam(L""));

  a = (wchar_t *)(L"{\"a\":\"hi\",\"b\":[1.2, 0.1, 100],\"x\":{\"x1\":null,\"x2\":true,\"x3\":false}}");
  aLength = wcslen(a);
  b = (wchar_t *)(L"{{}}]");
  bLength = wcslen(b);
  c = (wchar_t *)(L"");
  cLength = wcslen(c);
  d = (wchar_t *)(L"{");
  dLength = wcslen(d);

  AssertTrue(IsValidJSON(a, aLength, errorMessages), failures);
  AssertFalse(IsValidJSON(b, bLength, errorMessages), failures);
  AssertFalse(IsValidJSON(c, cLength, errorMessages), failures);
  AssertFalse(IsValidJSON(d, dLength, errorMessages), failures);
}
void testComparator(NumberReference *failures){
  wchar_t *a, *b;
  size_t aLength, bLength;
  StringArrayReference *errorMessages;
  BooleanReference *equalsReference;
  _Bool success;

  errorMessages = CreateStringArrayReferenceLengthValue(0.0, strparam(L""));
  equalsReference = CreateBooleanReference(false);

  a = (wchar_t *)(L"{\"a\":\"hi\",\"b\":[1.2, 0.1, 100],\"x\":{\"x1\":null,\"x2\":true,\"x3\":false}}");
  aLength = wcslen(a);
  b = (wchar_t *)(L"{\"x\":{\"x1\":null,\"x2\":true,\"x3\":false},\"a\":\"hi\",\"b\":[1.2, 0.1, 100]}");
  bLength = wcslen(b);

  success = JSONCompare(a, aLength, b, bLength, 0.0001, equalsReference, errorMessages);

  AssertTrue(success, failures);
  AssertTrue(equalsReference->booleanValue, failures);

  a = (wchar_t *)(L"{\"a\":\"hi\",\"b\":[1.201, 0.1, 100],\"x\":{\"x1\":null,\"x2\":true,\"x3\":false}}");
  aLength = wcslen(a);
  b = (wchar_t *)(L"{\"x\":{\"x1\":null,\"x2\":true,\"x3\":false},\"a\":\"hi\",\"b\":[1.2, 0.1, 100]}");
  bLength = wcslen(b);

  success = JSONCompare(a, aLength, b, bLength, 0.0001, equalsReference, errorMessages);

  AssertTrue(success, failures);
  AssertFalse(equalsReference->booleanValue, failures);

  success = JSONCompare(a, aLength, b, bLength, 0.1, equalsReference, errorMessages);

  AssertTrue(success, failures);
  AssertTrue(equalsReference->booleanValue, failures);
}
void testTokenizer1(NumberReference *failures){
  NumberReference *countReference, *stringLength;
  StringArrayReference *errorMessages, *tokenArrayReference;
  _Bool success;
  StringReference **numbers;
  size_t numbersLength;
  double i;

  countReference = CreateNumberReference(0.0);
  stringLength = CreateNumberReference(0.0);
  errorMessages = CreateStringArrayReferenceLengthValue(0.0, strparam(L""));

  tokenArrayReference = (StringArrayReference *)malloc(sizeof(StringArrayReference));

  success = JSONTokenize(strparam(L"false"), tokenArrayReference, errorMessages);
  AssertTrue(success, failures);
  if(success){
    AssertEquals(tokenArrayReference->stringArrayLength, 2.0, failures);
    AssertStringEquals(tokenArrayReference->stringArray[0]->string, tokenArrayReference->stringArray[0]->stringLength, strparam(L"false"), failures);
  }

  numbers = strSplitByString(&numbersLength, strparam(L"11, -1e-1, -0.123456789e-99, 1E1, -0.1E23"), strparam(L", "));

  for(i = 0.0; i < numbersLength; i = i + 1.0){
    success = JSONTokenize(numbers[(int)(i)]->string, numbers[(int)(i)]->stringLength, tokenArrayReference, errorMessages);
    AssertTrue(success, failures);
    if(success){
      AssertEquals(tokenArrayReference->stringArrayLength, 2.0, failures);
      AssertStringEquals(tokenArrayReference->stringArray[0]->string, tokenArrayReference->stringArray[0]->stringLength, numbers[(int)(i)]->string, numbers[(int)(i)]->stringLength, failures);
    }
  }

  success = IsValidJSONStringInJSON(strparam(L"\"\""), 0.0, countReference, stringLength, errorMessages);
  AssertTrue(success, failures);
  if(success){
    AssertEquals(countReference->numberValue, 2.0, failures);
  }

  success = IsValidJSONString(strparam(L"\"\\u1234\\n\\r\\f\\b\\t\""), errorMessages);
  AssertTrue(success, failures);

  success = JSONTokenize(strparam(L"\""), tokenArrayReference, errorMessages);
  AssertFalse(success, failures);

  success = IsValidJSONNumber(strparam(L"1.1-e1"), errorMessages);
  AssertFalse(success, failures);

  success = IsValidJSONNumber(strparam(L"1E+2"), errorMessages);
  AssertTrue(success, failures);

  success = IsValidJSONString(strparam(L"\"\\uAAAG\""), errorMessages);
  AssertFalse(success, failures);
}
BooleanReference *CreateBooleanReference(_Bool value){
  BooleanReference *ref;

  ref = (BooleanReference *)malloc(sizeof(BooleanReference));
  ref->booleanValue = value;

  return ref;
}
BooleanArrayReference *CreateBooleanArrayReference(_Bool *value, size_t valueLength){
  BooleanArrayReference *ref;

  ref = (BooleanArrayReference *)malloc(sizeof(BooleanArrayReference));
  ref->booleanArray = value;
  ref->booleanArrayLength = valueLength;

  return ref;
}
BooleanArrayReference *CreateBooleanArrayReferenceLengthValue(double length, _Bool value){
  BooleanArrayReference *ref;
  double i;

  ref = (BooleanArrayReference *)malloc(sizeof(BooleanArrayReference));
  ref->booleanArray = (_Bool*)malloc(sizeof(_Bool) * (length));
  ref->booleanArrayLength = length;

  for(i = 0.0; i < length; i = i + 1.0){
    ref->booleanArray[(int)(i)] = value;
  }

  return ref;
}
void FreeBooleanArrayReference(BooleanArrayReference *booleanArrayReference){
  free(booleanArrayReference->booleanArray);
  free(booleanArrayReference);
}
CharacterReference *CreateCharacterReference(wchar_t value){
  CharacterReference *ref;

  ref = (CharacterReference *)malloc(sizeof(CharacterReference));
  ref->characterValue = value;

  return ref;
}
NumberReference *CreateNumberReference(double value){
  NumberReference *ref;

  ref = (NumberReference *)malloc(sizeof(NumberReference));
  ref->numberValue = value;

  return ref;
}
NumberArrayReference *CreateNumberArrayReference(double *value, size_t valueLength){
  NumberArrayReference *ref;

  ref = (NumberArrayReference *)malloc(sizeof(NumberArrayReference));
  ref->numberArray = value;
  ref->numberArrayLength = valueLength;

  return ref;
}
NumberArrayReference *CreateNumberArrayReferenceLengthValue(double length, double value){
  NumberArrayReference *ref;
  double i;

  ref = (NumberArrayReference *)malloc(sizeof(NumberArrayReference));
  ref->numberArray = (double*)malloc(sizeof(double) * (length));
  ref->numberArrayLength = length;

  for(i = 0.0; i < length; i = i + 1.0){
    ref->numberArray[(int)(i)] = value;
  }

  return ref;
}
void FreeNumberArrayReference(NumberArrayReference *numberArrayReference){
  free(numberArrayReference->numberArray);
  free(numberArrayReference);
}
StringReference *CreateStringReference(wchar_t *value, size_t valueLength){
  StringReference *ref;

  ref = (StringReference *)malloc(sizeof(StringReference));
  ref->string = value;
  ref->stringLength = valueLength;

  return ref;
}
StringReference *CreateStringReferenceLengthValue(double length, wchar_t value){
  StringReference *ref;
  double i;

  ref = (StringReference *)malloc(sizeof(StringReference));
  ref->string = (wchar_t*)malloc(sizeof(wchar_t) * (length));
  ref->stringLength = length;

  for(i = 0.0; i < length; i = i + 1.0){
    ref->string[(int)(i)] = value;
  }

  return ref;
}
void FreeStringReference(StringReference *stringReference){
  free(stringReference->string);
  free(stringReference);
}
StringArrayReference *CreateStringArrayReference(StringReference **strings, size_t stringsLength){
  StringArrayReference *ref;

  ref = (StringArrayReference *)malloc(sizeof(StringArrayReference));
  ref->stringArray = strings;
  ref->stringArrayLength = stringsLength;

  return ref;
}
StringArrayReference *CreateStringArrayReferenceLengthValue(double length, wchar_t *value, size_t valueLength){
  StringArrayReference *ref;
  double i;

  ref = (StringArrayReference *)malloc(sizeof(StringArrayReference));
  ref->stringArray = (StringReference**)malloc(sizeof(StringReference) * length);
  ref->stringArrayLength = length;

  for(i = 0.0; i < length; i = i + 1.0){
    ref->stringArray[(int)(i)] = CreateStringReference(value, valueLength);
  }

  return ref;
}
void FreeStringArrayReference(StringArrayReference *stringArrayReference){
  double i;

  for(i = 0.0; i < stringArrayReference->stringArrayLength; i = i + 1.0){
    free(stringArrayReference->stringArray[(int)(i)]);
  }
  free(stringArrayReference->stringArray);
  free(stringArrayReference);
}
void strWriteStringToStingStream(wchar_t *stream, size_t streamLength, NumberReference *index, wchar_t *src, size_t srcLength){
  double i;

  for(i = 0.0; i < srcLength; i = i + 1.0){
    stream[(int)(index->numberValue + i)] = src[(int)(i)];
  }
  index->numberValue = index->numberValue + srcLength;
}
void strWriteCharacterToStingStream(wchar_t *stream, size_t streamLength, NumberReference *index, wchar_t src){
  stream[(int)(index->numberValue)] = src;
  index->numberValue = index->numberValue + 1.0;
}
void strWriteBooleanToStingStream(wchar_t *stream, size_t streamLength, NumberReference *index, _Bool src){
  if(src){
    strWriteStringToStingStream(stream, streamLength, index, strparam(L"true"));
  }else{
    strWriteStringToStingStream(stream, streamLength, index, strparam(L"false"));
  }
}
_Bool strSubstringWithCheck(wchar_t *string, size_t stringLength, double from, double to, StringReference *stringReference){
  _Bool success;

  if(from >= 0.0 && from <= stringLength && to >= 0.0 && to <= stringLength && from <= to){
    stringReference->string = strSubstring(&stringReference->stringLength, string, stringLength, from, to);
    success = true;
  }else{
    success = false;
  }

  return success;
}
wchar_t *strSubstring(size_t *returnArrayLength, wchar_t *string, size_t stringLength, double from, double to){
  wchar_t *n;
  size_t nLength;
  double i, length;

  length = to - from;

  n = (wchar_t*)malloc(sizeof(wchar_t) * (length));
  nLength = length;

  for(i = from; i < to; i = i + 1.0){
    n[(int)(i - from)] = string[(int)(i)];
  }

  *returnArrayLength = nLength;
  return n;
}
wchar_t *strAppendString(size_t *returnArrayLength, wchar_t *s1, size_t s1Length, wchar_t *s2, size_t s2Length){
  wchar_t *newString;
  size_t newStringLength;

  newString = strConcatenateString(&newStringLength, s1, s1Length, s2, s2Length);

  free(s1);

  *returnArrayLength = newStringLength;
  return newString;
}
wchar_t *strConcatenateString(size_t *returnArrayLength, wchar_t *s1, size_t s1Length, wchar_t *s2, size_t s2Length){
  wchar_t *newString;
  size_t newStringLength;
  double i;

  newString = (wchar_t*)malloc(sizeof(wchar_t) * (s1Length + s2Length));
  newStringLength = s1Length + s2Length;

  for(i = 0.0; i < s1Length; i = i + 1.0){
    newString[(int)(i)] = s1[(int)(i)];
  }

  for(i = 0.0; i < s2Length; i = i + 1.0){
    newString[(int)(s1Length + i)] = s2[(int)(i)];
  }

  *returnArrayLength = newStringLength;
  return newString;
}
wchar_t *strAppendCharacter(size_t *returnArrayLength, wchar_t *string, size_t stringLength, wchar_t c){
  wchar_t *newString;
  size_t newStringLength;

  newString = strConcatenateCharacter(&newStringLength, string, stringLength, c);

  free(string);

  *returnArrayLength = newStringLength;
  return newString;
}
wchar_t *strConcatenateCharacter(size_t *returnArrayLength, wchar_t *string, size_t stringLength, wchar_t c){
  wchar_t *newString;
  size_t newStringLength;
  double i;
  newString = (wchar_t*)malloc(sizeof(wchar_t) * (stringLength + 1.0));
  newStringLength = stringLength + 1.0;

  for(i = 0.0; i < stringLength; i = i + 1.0){
    newString[(int)(i)] = string[(int)(i)];
  }

  newString[(int)(stringLength)] = c;

  *returnArrayLength = newStringLength;
  return newString;
}
StringReference **strSplitByCharacter(size_t *returnArrayLength, wchar_t *toSplit, size_t toSplitLength, wchar_t splitBy){
  StringReference **split;
  size_t splitLength;
  wchar_t *stringToSplitBy;
  size_t stringToSplitByLength;

  stringToSplitBy = (wchar_t*)malloc(sizeof(wchar_t) * (1.0));
  stringToSplitByLength = 1.0;
  stringToSplitBy[0] = splitBy;

  split = strSplitByString(&splitLength, toSplit, toSplitLength, stringToSplitBy, stringToSplitByLength);

  free(stringToSplitBy);

  *returnArrayLength = splitLength;
  return split;
}
_Bool strIndexOfCharacter(wchar_t *string, size_t stringLength, wchar_t character, NumberReference *indexReference){
  double i;
  _Bool found;

  found = false;
  for(i = 0.0; i < stringLength &&  !found ; i = i + 1.0){
    if(string[(int)(i)] == character){
      found = true;
      indexReference->numberValue = i;
    }
  }

  return found;
}
_Bool strSubstringEqualsWithCheck(wchar_t *string, size_t stringLength, double from, wchar_t *substring, size_t substringLength, BooleanReference *equalsReference){
  _Bool success;

  if(from < stringLength){
    success = true;
    equalsReference->booleanValue = strSubstringEquals(string, stringLength, from, substring, substringLength);
  }else{
    success = false;
  }

  return success;
}
_Bool strSubstringEquals(wchar_t *string, size_t stringLength, double from, wchar_t *substring, size_t substringLength){
  double i;
  _Bool equal;

  equal = true;
  if(stringLength - from >= substringLength){
    for(i = 0.0; i < substringLength && equal; i = i + 1.0){
      if(string[(int)(from + i)] != substring[(int)(i)]){
        equal = false;
      }
    }
  }else{
    equal = false;
  }

  return equal;
}
_Bool strIndexOfString(wchar_t *string, size_t stringLength, wchar_t *substring, size_t substringLength, NumberReference *indexReference){
  double i;
  _Bool found;

  found = false;
  for(i = 0.0; i < stringLength - substringLength + 1.0 &&  !found ; i = i + 1.0){
    if(strSubstringEquals(string, stringLength, i, substring, substringLength)){
      found = true;
      indexReference->numberValue = i;
    }
  }

  return found;
}
_Bool strContainsCharacter(wchar_t *string, size_t stringLength, wchar_t character){
  double i;
  _Bool found;

  found = false;
  for(i = 0.0; i < stringLength &&  !found ; i = i + 1.0){
    if(string[(int)(i)] == character){
      found = true;
    }
  }

  return found;
}
_Bool strContainsString(wchar_t *string, size_t stringLength, wchar_t *substring, size_t substringLength){
  return strIndexOfString(string, stringLength, substring, substringLength, (NumberReference *)malloc(sizeof(NumberReference)));
}
void strToUpperCase(wchar_t *string, size_t stringLength){
  double i;

  for(i = 0.0; i < stringLength; i = i + 1.0){
    string[(int)(i)] = charToUpperCase(string[(int)(i)]);
  }
}
void strToLowerCase(wchar_t *string, size_t stringLength){
  double i;

  for(i = 0.0; i < stringLength; i = i + 1.0){
    string[(int)(i)] = charToLowerCase(string[(int)(i)]);
  }
}
_Bool strEqualsIgnoreCase(wchar_t *a, size_t aLength, wchar_t *b, size_t bLength){
  _Bool equal;
  double i;

  if(aLength == bLength){
    equal = true;
    for(i = 0.0; i < aLength && equal; i = i + 1.0){
      if(charToLowerCase(a[(int)(i)]) != charToLowerCase(b[(int)(i)])){
        equal = false;
      }
    }
  }else{
    equal = false;
  }

  return equal;
}
wchar_t *strReplaceString(size_t *returnArrayLength, wchar_t *string, size_t stringLength, wchar_t *toReplace, size_t toReplaceLength, wchar_t *replaceWith, size_t replaceWithLength){
  wchar_t *result;
  size_t resultLength;
  double i, j;
  BooleanReference *equalsReference;
  _Bool success;
  DynamicArrayCharacters *da;

  da = CreateDynamicArrayCharacters();

  equalsReference = (BooleanReference *)malloc(sizeof(BooleanReference));

  for(i = 0.0; i < stringLength; ){
    success = strSubstringEqualsWithCheck(string, stringLength, i, toReplace, toReplaceLength, equalsReference);
    if(success){
      success = equalsReference->booleanValue;
    }

    if(success && toReplaceLength > 0.0){
      for(j = 0.0; j < replaceWithLength; j = j + 1.0){
        DynamicArrayAddCharacter(da, replaceWith[(int)(j)]);
      }
      i = i + toReplaceLength;
    }else{
      DynamicArrayAddCharacter(da, string[(int)(i)]);
      i = i + 1.0;
    }
  }

  result = DynamicArrayCharactersToArray(&resultLength, da);

  FreeDynamicArrayCharacters(da);

  *returnArrayLength = resultLength;
  return result;
}
wchar_t *strReplaceCharacterToNew(size_t *returnArrayLength, wchar_t *string, size_t stringLength, wchar_t toReplace, wchar_t replaceWith){
  wchar_t *result;
  size_t resultLength;
  double i;

  result = (wchar_t*)malloc(sizeof(wchar_t) * (stringLength));
  resultLength = stringLength;

  for(i = 0.0; i < stringLength; i = i + 1.0){
    if(string[(int)(i)] == toReplace){
      result[(int)(i)] = replaceWith;
    }else{
      result[(int)(i)] = string[(int)(i)];
    }
  }

  *returnArrayLength = resultLength;
  return result;
}
void strReplaceCharacter(wchar_t *string, size_t stringLength, wchar_t toReplace, wchar_t replaceWith){
  double i;

  for(i = 0.0; i < stringLength; i = i + 1.0){
    if(string[(int)(i)] == toReplace){
      string[(int)(i)] = replaceWith;
    }
  }
}
wchar_t *strTrim(size_t *returnArrayLength, wchar_t *string, size_t stringLength){
  wchar_t *result;
  size_t resultLength;
  double i, lastWhitespaceLocationStart, lastWhitespaceLocationEnd;
  _Bool firstNonWhitespaceFound;

  /* Find whitepaces at the start. */
  lastWhitespaceLocationStart =  -1.0;
  firstNonWhitespaceFound = false;
  for(i = 0.0; i < stringLength &&  !firstNonWhitespaceFound ; i = i + 1.0){
    if(charIsWhiteSpace(string[(int)(i)])){
      lastWhitespaceLocationStart = i;
    }else{
      firstNonWhitespaceFound = true;
    }
  }

  /* Find whitepaces at the end. */
  lastWhitespaceLocationEnd = stringLength;
  firstNonWhitespaceFound = false;
  for(i = stringLength - 1.0; i >= 0.0 &&  !firstNonWhitespaceFound ; i = i - 1.0){
    if(charIsWhiteSpace(string[(int)(i)])){
      lastWhitespaceLocationEnd = i;
    }else{
      firstNonWhitespaceFound = true;
    }
  }

  if(lastWhitespaceLocationStart < lastWhitespaceLocationEnd){
    result = strSubstring(&resultLength, string, stringLength, lastWhitespaceLocationStart + 1.0, lastWhitespaceLocationEnd);
  }else{
    result = (wchar_t*)malloc(sizeof(wchar_t) * (0.0));
    resultLength = 0.0;
  }

  *returnArrayLength = resultLength;
  return result;
}
_Bool strStartsWith(wchar_t *string, size_t stringLength, wchar_t *start, size_t startLength){
  _Bool startsWithString;

  startsWithString = false;
  if(stringLength >= startLength){
    startsWithString = strSubstringEquals(string, stringLength, 0.0, start, startLength);
  }

  return startsWithString;
}
_Bool strEndsWith(wchar_t *string, size_t stringLength, wchar_t *end, size_t endLength){
  _Bool endsWithString;

  endsWithString = false;
  if(stringLength >= endLength){
    endsWithString = strSubstringEquals(string, stringLength, stringLength - endLength, end, endLength);
  }

  return endsWithString;
}
StringReference **strSplitByString(size_t *returnArrayLength, wchar_t *toSplit, size_t toSplitLength, wchar_t *splitBy, size_t splitByLength){
  StringReference **split;
  size_t splitLength;
  wchar_t *next;
  size_t nextLength;
  double i;
  wchar_t c;
  StringReference *n;

  split = (StringReference**)malloc(sizeof(StringReference) * 0.0);
  splitLength = 0.0;

  next = (wchar_t*)malloc(sizeof(wchar_t) * (0.0));
  nextLength = 0.0;
  for(i = 0.0; i < toSplitLength; ){
    c = toSplit[(int)(i)];

    if(strSubstringEquals(toSplit, toSplitLength, i, splitBy, splitByLength)){
      n = (StringReference *)malloc(sizeof(StringReference));
      n->string = next;
      n->stringLength = nextLength;
      split = AddString(&splitLength, split, splitLength, n);
      next = (wchar_t*)malloc(sizeof(wchar_t) * (0.0));
      nextLength = 0.0;
      i = i + splitByLength;
    }else{
      next = strAppendCharacter(&nextLength, next, nextLength, c);
      i = i + 1.0;
    }
  }

  n = (StringReference *)malloc(sizeof(StringReference));
  n->string = next;
  n->stringLength = nextLength;
  split = AddString(&splitLength, split, splitLength, n);

  *returnArrayLength = splitLength;
  return split;
}
_Bool strStringIsBefore(wchar_t *a, size_t aLength, wchar_t *b, size_t bLength){
  _Bool before, equal, done;
  double i;

  before = false;
  equal = true;
  done = false;

  if(aLength == 0.0 && bLength > 0.0){
    before = true;
  }else{
    for(i = 0.0; i < aLength && i < bLength &&  !done ; i = i + 1.0){
      if(a[(int)(i)] != b[(int)(i)]){
        equal = false;
      }
      if(charCharacterIsBefore(a[(int)(i)], b[(int)(i)])){
        before = true;
      }
      if(charCharacterIsBefore(b[(int)(i)], a[(int)(i)])){
        done = true;
      }
    }

    if(equal){
      if(aLength < bLength){
        before = true;
      }
    }
  }

  return before;
}
wchar_t *strJoinStringsWithSeparator(size_t *returnArrayLength, StringReference **strings, size_t stringsLength, wchar_t *separator, size_t separatorLength){
  wchar_t *result, *string;
  size_t resultLength, stringLength;
  double length, i;
  NumberReference *index;

  index = CreateNumberReference(0.0);

  length = 0.0;
  for(i = 0.0; i < stringsLength; i = i + 1.0){
    length = length + strings[(int)(i)]->stringLength;
  }
  length = length + (stringsLength - 1.0)*separatorLength;

  result = (wchar_t*)malloc(sizeof(wchar_t) * (length));
  resultLength = length;

  for(i = 0.0; i < stringsLength; i = i + 1.0){
    string = strings[(int)(i)]->string;
    stringLength = strings[(int)(i)]->stringLength;
    strWriteStringToStingStream(result, resultLength, index, string, stringLength);
    if(i + 1.0 < stringsLength){
      strWriteStringToStingStream(result, resultLength, index, separator, separatorLength);
    }
  }

  free(index);

  *returnArrayLength = resultLength;
  return result;
}
wchar_t *strJoinStrings(size_t *returnArrayLength, StringReference **strings, size_t stringsLength){
  wchar_t *result, *string;
  size_t resultLength, stringLength;
  double length, i;
  NumberReference *index;

  index = CreateNumberReference(0.0);

  length = 0.0;
  for(i = 0.0; i < stringsLength; i = i + 1.0){
    length = length + strings[(int)(i)]->stringLength;
  }

  result = (wchar_t*)malloc(sizeof(wchar_t) * (length));
  resultLength = length;

  for(i = 0.0; i < stringsLength; i = i + 1.0){
    string = strings[(int)(i)]->string;
    stringLength = strings[(int)(i)]->stringLength;
    strWriteStringToStingStream(result, resultLength, index, string, stringLength);
  }

  free(index);

  *returnArrayLength = resultLength;
  return result;
}
wchar_t *nCreateStringScientificNotationDecimalFromNumber(size_t *returnArrayLength, double decimal){
  StringReference *mantissaReference, *exponentReference;
  double multiplier, inc;
  double exponent;
  _Bool done, isPositive;
  wchar_t *result;
  size_t resultLength;

  mantissaReference = (StringReference *)malloc(sizeof(StringReference));
  exponentReference = (StringReference *)malloc(sizeof(StringReference));
  result = (wchar_t*)malloc(sizeof(wchar_t) * (0.0));
  resultLength = 0.0;
  done = false;
  exponent = 0.0;

  if(decimal < 0.0){
    isPositive = false;
    decimal =  -decimal;
  }else{
    isPositive = true;
  }

  if(decimal == 0.0){
    done = true;
  }

  if( !done ){
    multiplier = 0.0;
    inc = 0.0;

    if(decimal < 1.0){
      multiplier = 10.0;
      inc =  -1.0;
    }else if(decimal >= 10.0){
      multiplier = 0.1;
      inc = 1.0;
    }else{
      done = true;
    }

    if( !done ){
      exponent = round(log10(decimal));
      exponent = fmin(99.0, exponent);
      exponent = fmax( -99.0, exponent);

      decimal = decimal/pow(10.0, exponent);

      /* Adjust */
      for(; (decimal >= 10.0 || decimal < 1.0) && fabs(exponent) < 99.0; ){
        decimal = decimal*multiplier;
        exponent = exponent + inc;
      }
    }
  }

  nCreateStringFromNumberWithCheck(decimal, 10.0, mantissaReference);

  nCreateStringFromNumberWithCheck(exponent, 10.0, exponentReference);

  if( !isPositive ){
    result = strAppendString(&resultLength, result, resultLength, strparam(L"-"));
  }

  result = strAppendString(&resultLength, result, resultLength, mantissaReference->string, mantissaReference->stringLength);
  result = strAppendString(&resultLength, result, resultLength, strparam(L"e"));
  result = strAppendString(&resultLength, result, resultLength, exponentReference->string, exponentReference->stringLength);

  *returnArrayLength = resultLength;
  return result;
}
wchar_t *nCreateStringDecimalFromNumber(size_t *returnArrayLength, double decimal){
  wchar_t *returnString;
  StringReference *stringReference;

  stringReference = (StringReference *)malloc(sizeof(StringReference));

  /* This will succeed because base = 10. */
  nCreateStringFromNumberWithCheck(decimal, 10.0, stringReference);

  *returnArrayLength = stringReference->stringLength;
  returnString = stringReference->string;
  free(stringReference);
  return returnString;
}
_Bool nCreateStringFromNumberWithCheck(double decimal, double base, StringReference *stringReference){
  DynamicArrayCharacters *string;
  double maximumDigits;
  double digitPosition;
  _Bool hasPrintedPoint, isPositive;
  double i, d;
  _Bool success;
  CharacterReference *characterReference;
  wchar_t c;

  string = CreateDynamicArrayCharacters();
  isPositive = true;

  if(decimal < 0.0){
    isPositive = false;
    decimal =  -decimal;
  }

  if(decimal == 0.0){
    DynamicArrayAddCharacter(string, '0');
    success = true;
  }else{
    characterReference = (CharacterReference *)malloc(sizeof(CharacterReference));

    if(IsInteger(base)){
      success = true;

      maximumDigits = nGetMaximumDigitsForBase(base);

      digitPosition = nGetFirstDigitPosition(decimal, base);

      decimal = round(decimal*pow(base, maximumDigits - digitPosition - 1.0));

      hasPrintedPoint = false;

      if( !isPositive ){
        DynamicArrayAddCharacter(string, '-');
      }

      /* Print leading zeros. */
      if(digitPosition < 0.0){
        DynamicArrayAddCharacter(string, '0');
        DynamicArrayAddCharacter(string, '.');
        hasPrintedPoint = true;
        for(i = 0.0; i <  -digitPosition - 1.0; i = i + 1.0){
          DynamicArrayAddCharacter(string, '0');
        }
      }

      /* Print number. */
      for(i = 0.0; i < maximumDigits && success; i = i + 1.0){
        d = floor(decimal/pow(base, maximumDigits - i - 1.0));

        if(d >= base){
          d = base - 1.0;
        }

        if( !hasPrintedPoint  && digitPosition - i + 1.0 == 0.0){
          if(decimal != 0.0){
            DynamicArrayAddCharacter(string, '.');
          }
          hasPrintedPoint = true;
        }

        if(decimal == 0.0 && hasPrintedPoint){
        }else{
          success = nGetSingleDigitCharacterFromNumberWithCheck(d, base, characterReference);
          if(success){
            c = characterReference->characterValue;
            DynamicArrayAddCharacter(string, c);
          }
        }

        if(success){
          decimal = decimal - d*pow(base, maximumDigits - i - 1.0);
          decimal = fmax(decimal, 0.0);
          decimal = round(decimal);
        }
      }

      if(success){
        /* Print trailing zeros. */
        for(i = 0.0; i < digitPosition - maximumDigits + 1.0; i = i + 1.0){
          DynamicArrayAddCharacter(string, '0');
        }
      }
    }else{
      success = false;
    }
    free(characterReference);
  }

  if(success){
    stringReference->string = DynamicArrayCharactersToArray(&stringReference->stringLength, string);
    FreeDynamicArrayCharacters(string);
  }

  /* Done */
  return success;
}
double nGetMaximumDigitsForBase(double base){
  double t;

  t = pow(10.0, 15.0);
  return floor(log10(t)/log10(base));
}
double nGetFirstDigitPosition(double decimal, double base){
  double power;
  double t;

  power = ceil(log10(decimal)/log10(base));

  t = decimal*pow(base,  -power);
  if(t < base && t >= 1.0){
  }else if(t >= base){
    power = power + 1.0;
  }else if(t < 1.0){
    power = power - 1.0;
  }

  return power;
}
_Bool nGetSingleDigitCharacterFromNumberWithCheck(double c, double base, CharacterReference *characterReference){
  wchar_t *numberTable;
  size_t numberTableLength;
  _Bool success;

  numberTable = nGetDigitCharacterTable(&numberTableLength);

  if(c < base || c < numberTableLength){
    success = true;
    characterReference->characterValue = numberTable[(int)(c)];
  }else{
    success = false;
  }

  return success;
}
wchar_t *nGetDigitCharacterTable(size_t *returnArrayLength){
  wchar_t *numberTable;
  size_t numberTableLength;

  numberTable = (wchar_t *)(L"0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ");
  numberTableLength = wcslen(numberTable);

  *returnArrayLength = numberTableLength;
  return numberTable;
}
_Bool nCreateNumberFromDecimalStringWithCheck(wchar_t *string, size_t stringLength, NumberReference *decimalReference, StringReference *errorMessage){
  return nCreateNumberFromStringWithCheck(string, stringLength, 10.0, decimalReference, errorMessage);
}
double nCreateNumberFromDecimalString(wchar_t *string, size_t stringLength){
  NumberReference *doubleReference;
  StringReference *stringReference;
  double number;

  doubleReference = CreateNumberReference(0.0);
  stringReference = CreateStringReference(strparam(L""));
  nCreateNumberFromStringWithCheck(string, stringLength, 10.0, doubleReference, stringReference);
  number = doubleReference->numberValue;

  free(doubleReference);
  free(stringReference);

  return number;
}
_Bool nCreateNumberFromStringWithCheck(wchar_t *string, size_t stringLength, double base, NumberReference *numberReference, StringReference *errorMessage){
  _Bool success;
  BooleanReference *numberIsPositive, *exponentIsPositive;
  NumberArrayReference *beforePoint, *afterPoint, *exponent;

  numberIsPositive = CreateBooleanReference(true);
  exponentIsPositive = CreateBooleanReference(true);
  beforePoint = (NumberArrayReference *)malloc(sizeof(NumberArrayReference));
  afterPoint = (NumberArrayReference *)malloc(sizeof(NumberArrayReference));
  exponent = (NumberArrayReference *)malloc(sizeof(NumberArrayReference));

  if(base >= 2.0 && base <= 36.0){
    success = nExtractPartsFromNumberString(string, stringLength, base, numberIsPositive, beforePoint, afterPoint, exponentIsPositive, exponent, errorMessage);

    if(success){
      numberReference->numberValue = nCreateNumberFromParts(base, numberIsPositive->booleanValue, beforePoint->numberArray, beforePoint->numberArrayLength, afterPoint->numberArray, afterPoint->numberArrayLength, exponentIsPositive->booleanValue, exponent->numberArray, exponent->numberArrayLength);
    }
  }else{
    success = false;
    errorMessage->string = (wchar_t *)(L"Base must be from 2 to 36.");
    errorMessage->stringLength = wcslen(errorMessage->string);
  }
  FreeNumberArrayReference(beforePoint);
  FreeNumberArrayReference(afterPoint);
  FreeNumberArrayReference(exponent);
  free(numberIsPositive);
  free(exponentIsPositive);

  return success;
}
double nCreateNumberFromParts(double base, _Bool numberIsPositive, double *beforePoint, size_t beforePointLength, double *afterPoint, size_t afterPointLength, _Bool exponentIsPositive, double *exponent, size_t exponentLength){
  double n, i, p, e;

  n = 0.0;

  for(i = 0.0; i < beforePointLength; i = i + 1.0){
    p = beforePoint[(int)(beforePointLength - i - 1.0)];

    n = n + p*pow(base, i);
  }

  for(i = 0.0; i < afterPointLength; i = i + 1.0){
    p = afterPoint[(int)(i)];

    n = n + p*pow(base,  -(i + 1.0));
  }

  if(exponentLength > 0.0){
    e = 0.0;
    for(i = 0.0; i < exponentLength; i = i + 1.0){
      p = exponent[(int)(exponentLength - i - 1.0)];

      e = e + p*pow(base, i);
    }

    if( !exponentIsPositive ){
      e =  -e;
    }

    n = n*pow(base, e);
  }

  if( !numberIsPositive ){
    n =  -n;
  }

  return n;
}
_Bool nExtractPartsFromNumberString(wchar_t *n, size_t nLength, double base, BooleanReference *numberIsPositive, NumberArrayReference *beforePoint, NumberArrayReference *afterPoint, BooleanReference *exponentIsPositive, NumberArrayReference *exponent, StringReference *errorMessages){
  double i, j, count;
  _Bool success, done, complete;

  i = 0.0;
  complete = false;

  if(i < nLength){
    if(n[(int)(i)] == '-'){
      numberIsPositive->booleanValue = false;
      i = i + 1.0;
    }else if(n[(int)(i)] == '+'){
      numberIsPositive->booleanValue = true;
      i = i + 1.0;
    }

    success = true;
  }else{
    success = false;
    errorMessages->string = (wchar_t *)(L"Number cannot have length zero.");
    errorMessages->stringLength = wcslen(errorMessages->string);
  }

  if(success){
    done = false;
    count = 0.0;
    for(; i + count < nLength &&  !done ; ){
      if(nCharacterIsNumberCharacterInBase(n[(int)(i + count)], base)){
        count = count + 1.0;
      }else{
        done = true;
      }
    }

    if(count >= 1.0){
      beforePoint->numberArray = (double*)malloc(sizeof(double) * (count));
      beforePoint->numberArrayLength = count;

      for(j = 0.0; j < count; j = j + 1.0){
        beforePoint->numberArray[(int)(j)] = nGetNumberFromNumberCharacterForBase(n[(int)(i + j)], base);
      }

      i = i + count;

      if(i < nLength){
        success = true;
      }else{
        afterPoint->numberArray = (double*)malloc(sizeof(double) * (0.0));
        afterPoint->numberArrayLength = 0.0;
        exponent->numberArray = (double*)malloc(sizeof(double) * (0.0));
        exponent->numberArrayLength = 0.0;
        success = true;
        complete = true;
      }
    }else{
      success = false;
      errorMessages->string = (wchar_t *)(L"Number must have at least one number after the optional sign.");
      errorMessages->stringLength = wcslen(errorMessages->string);
    }
  }

  if(success &&  !complete ){
    if(n[(int)(i)] == '.'){
      i = i + 1.0;

      if(i < nLength){
        done = false;
        count = 0.0;
        for(; i + count < nLength &&  !done ; ){
          if(nCharacterIsNumberCharacterInBase(n[(int)(i + count)], base)){
            count = count + 1.0;
          }else{
            done = true;
          }
        }

        if(count >= 1.0){
          afterPoint->numberArray = (double*)malloc(sizeof(double) * (count));
          afterPoint->numberArrayLength = count;

          for(j = 0.0; j < count; j = j + 1.0){
            afterPoint->numberArray[(int)(j)] = nGetNumberFromNumberCharacterForBase(n[(int)(i + j)], base);
          }

          i = i + count;

          if(i < nLength){
            success = true;
          }else{
            exponent->numberArray = (double*)malloc(sizeof(double) * (0.0));
            exponent->numberArrayLength = 0.0;
            success = true;
            complete = true;
          }
        }else{
          success = false;
          errorMessages->string = (wchar_t *)(L"There must be at least one digit after the decimal point.");
          errorMessages->stringLength = wcslen(errorMessages->string);
        }
      }else{
        success = false;
        errorMessages->string = (wchar_t *)(L"There must be at least one digit after the decimal point.");
        errorMessages->stringLength = wcslen(errorMessages->string);
      }
    }else if(base <= 14.0 && (n[(int)(i)] == 'e' || n[(int)(i)] == 'E')){
      if(i < nLength){
        success = true;
        afterPoint->numberArray = (double*)malloc(sizeof(double) * (0.0));
        afterPoint->numberArrayLength = 0.0;
      }else{
        success = false;
        errorMessages->string = (wchar_t *)(L"There must be at least one digit after the exponent.");
        errorMessages->stringLength = wcslen(errorMessages->string);
      }
    }else{
      success = false;
      errorMessages->string = (wchar_t *)(L"Expected decimal point or exponent symbol.");
      errorMessages->stringLength = wcslen(errorMessages->string);
    }
  }

  if(success &&  !complete ){
    if(base <= 14.0 && (n[(int)(i)] == 'e' || n[(int)(i)] == 'E')){
      i = i + 1.0;

      if(i < nLength){
        if(n[(int)(i)] == '-'){
          exponentIsPositive->booleanValue = false;
          i = i + 1.0;
        }else if(n[(int)(i)] == '+'){
          exponentIsPositive->booleanValue = true;
          i = i + 1.0;
        }

        if(i < nLength){
          done = false;
          count = 0.0;
          for(; i + count < nLength &&  !done ; ){
            if(nCharacterIsNumberCharacterInBase(n[(int)(i + count)], base)){
              count = count + 1.0;
            }else{
              done = true;
            }
          }

          if(count >= 1.0){
            exponent->numberArray = (double*)malloc(sizeof(double) * (count));
            exponent->numberArrayLength = count;

            for(j = 0.0; j < count; j = j + 1.0){
              exponent->numberArray[(int)(j)] = nGetNumberFromNumberCharacterForBase(n[(int)(i + j)], base);
            }

            i = i + count;

            if(i == nLength){
              success = true;
            }else{
              success = false;
              errorMessages->string = (wchar_t *)(L"There cannot be any characters past the exponent of the number.");
              errorMessages->stringLength = wcslen(errorMessages->string);
            }
          }else{
            success = false;
            errorMessages->string = (wchar_t *)(L"There must be at least one digit after the decimal point.");
            errorMessages->stringLength = wcslen(errorMessages->string);
          }
        }else{
          success = false;
          errorMessages->string = (wchar_t *)(L"There must be at least one digit after the exponent symbol.");
          errorMessages->stringLength = wcslen(errorMessages->string);
        }
      }else{
        success = false;
        errorMessages->string = (wchar_t *)(L"There must be at least one digit after the exponent symbol.");
        errorMessages->stringLength = wcslen(errorMessages->string);
      }
    }else{
      success = false;
      errorMessages->string = (wchar_t *)(L"Expected exponent symbol.");
      errorMessages->stringLength = wcslen(errorMessages->string);
    }
  }

  return success;
}
double nGetNumberFromNumberCharacterForBase(wchar_t c, double base){
  wchar_t *numberTable;
  size_t numberTableLength;
  double i;
  double position;

  numberTable = nGetDigitCharacterTable(&numberTableLength);
  position = 0.0;

  for(i = 0.0; i < base; i = i + 1.0){
    if(numberTable[(int)(i)] == c){
      position = i;
    }
  }

  return position;
}
_Bool nCharacterIsNumberCharacterInBase(wchar_t c, double base){
  wchar_t *numberTable;
  size_t numberTableLength;
  double i;
  _Bool found;

  numberTable = nGetDigitCharacterTable(&numberTableLength);
  found = false;

  for(i = 0.0; i < base; i = i + 1.0){
    if(numberTable[(int)(i)] == c){
      found = true;
    }
  }

  return found;
}
double *nStringToNumberArray(size_t *returnArrayLength, wchar_t *str, size_t strLength){
  NumberArrayReference *numberArrayReference;
  StringReference *stringReference;
  double *numbers;
  size_t numbersLength;

  numberArrayReference = (NumberArrayReference *)malloc(sizeof(NumberArrayReference));
  stringReference = (StringReference *)malloc(sizeof(StringReference));

  nStringToNumberArrayWithCheck(str, strLength, numberArrayReference, stringReference);

  numbers = numberArrayReference->numberArray;
  numbersLength = numberArrayReference->numberArrayLength;

  free(numberArrayReference);
  free(stringReference);

  *returnArrayLength = numbersLength;
  return numbers;
}
_Bool nStringToNumberArrayWithCheck(wchar_t *str, size_t strLength, NumberArrayReference *numberArrayReference, StringReference *errorMessage){
  StringReference **numberStrings;
  size_t numberStringsLength;
  double *numbers;
  size_t numbersLength;
  double i;
  wchar_t *numberString, *trimmedNumberString;
  size_t numberStringLength, trimmedNumberStringLength;
  _Bool success;
  NumberReference *numberReference;

  numberStrings = strSplitByString(&numberStringsLength, str, strLength, strparam(L","));

  numbers = (double*)malloc(sizeof(double) * (numberStringsLength));
  numbersLength = numberStringsLength;
  success = true;
  numberReference = (NumberReference *)malloc(sizeof(NumberReference));

  for(i = 0.0; i < numberStringsLength; i = i + 1.0){
    numberString = numberStrings[(int)(i)]->string;
    numberStringLength = numberStrings[(int)(i)]->stringLength;
    trimmedNumberString = strTrim(&trimmedNumberStringLength, numberString, numberStringLength);
    success = nCreateNumberFromDecimalStringWithCheck(trimmedNumberString, trimmedNumberStringLength, numberReference, errorMessage);
    numbers[(int)(i)] = numberReference->numberValue;

    FreeStringReference(numberStrings[(int)(i)]);
    free(trimmedNumberString);
  }

  free(numberStrings);
  free(numberReference);

  numberArrayReference->numberArray = numbers;
  numberArrayReference->numberArrayLength = numbersLength;

  return success;
}
double *AddNumber(size_t *returnArrayLength, double *list, size_t listLength, double a){
  double *newlist;
  size_t newlistLength;
  double i;

  newlist = (double*)malloc(sizeof(double) * (listLength + 1.0));
  newlistLength = listLength + 1.0;
  for(i = 0.0; i < listLength; i = i + 1.0){
    newlist[(int)(i)] = list[(int)(i)];
  }
  newlist[(int)(listLength)] = a;
		
  free(list);
		
  *returnArrayLength = newlistLength;
  return newlist;
}
void AddNumberRef(NumberArrayReference *list, double i){
  list->numberArray = AddNumber(&list->numberArrayLength, list->numberArray, list->numberArrayLength, i);
}
double *RemoveNumber(size_t *returnArrayLength, double *list, size_t listLength, double n){
  double *newlist;
  size_t newlistLength;
  double i;

  newlist = (double*)malloc(sizeof(double) * (listLength - 1.0));
  newlistLength = listLength - 1.0;

  if(n >= 0.0 && n < listLength){
    for(i = 0.0; i < listLength; i = i + 1.0){
      if(i < n){
        newlist[(int)(i)] = list[(int)(i)];
      }
      if(i > n){
        newlist[(int)(i - 1.0)] = list[(int)(i)];
      }
    }

    free(list);
  }else{
    free(newlist);
  }
		
  *returnArrayLength = newlistLength;
  return newlist;
}
double GetNumberRef(NumberArrayReference *list, double i){
  return list->numberArray[(int)(i)];
}
void RemoveNumberRef(NumberArrayReference *list, double i){
  list->numberArray = RemoveNumber(&list->numberArrayLength, list->numberArray, list->numberArrayLength, i);
}
StringReference **AddString(size_t *returnArrayLength, StringReference **list, size_t listLength, StringReference *a){
  StringReference **newlist;
  size_t newlistLength;
  double i;

  newlist = (StringReference**)malloc(sizeof(StringReference) * listLength + 1.0);
  newlistLength = listLength + 1.0;

  for(i = 0.0; i < listLength; i = i + 1.0){
    newlist[(int)(i)] = list[(int)(i)];
  }
  newlist[(int)(listLength)] = a;
		
  free(list);
		
  *returnArrayLength = newlistLength;
  return newlist;
}
void AddStringRef(StringArrayReference *list, StringReference *i){
  list->stringArray = AddString(&list->stringArrayLength, list->stringArray, list->stringArrayLength, i);
}
StringReference **RemoveString(size_t *returnArrayLength, StringReference **list, size_t listLength, double n){
  StringReference **newlist;
  size_t newlistLength;
  double i;

  newlist = (StringReference**)malloc(sizeof(StringReference) * listLength - 1.0);
  newlistLength = listLength - 1.0;

  if(n >= 0.0 && n < listLength){
    for(i = 0.0; i < listLength; i = i + 1.0){
      if(i < n){
        newlist[(int)(i)] = list[(int)(i)];
      }
      if(i > n){
        newlist[(int)(i - 1.0)] = list[(int)(i)];
      }
    }

    free(list);
  }else{
    free(newlist);
  }
		
  *returnArrayLength = newlistLength;
  return newlist;
}
StringReference *GetStringRef(StringArrayReference *list, double i){
  return list->stringArray[(int)(i)];
}
void RemoveStringRef(StringArrayReference *list, double i){
  list->stringArray = RemoveString(&list->stringArrayLength, list->stringArray, list->stringArrayLength, i);
}
DynamicArrayCharacters *CreateDynamicArrayCharacters(){
  DynamicArrayCharacters *da;

  da = (DynamicArrayCharacters *)malloc(sizeof(DynamicArrayCharacters));
  da->array = (wchar_t*)malloc(sizeof(wchar_t) * (10.0));
  da->arrayLength = 10.0;
  da->length = 0.0;

  return da;
}
DynamicArrayCharacters *CreateDynamicArrayCharactersWithInitialCapacity(double capacity){
  DynamicArrayCharacters *da;

  da = (DynamicArrayCharacters *)malloc(sizeof(DynamicArrayCharacters));
  da->array = (wchar_t*)malloc(sizeof(wchar_t) * (capacity));
  da->arrayLength = capacity;
  da->length = 0.0;

  return da;
}
void DynamicArrayAddCharacter(DynamicArrayCharacters *da, wchar_t value){
  if(da->length == da->arrayLength){
    DynamicArrayCharactersIncreaseSize(da);
  }

  da->array[(int)(da->length)] = value;
  da->length = da->length + 1.0;
}
void DynamicArrayCharactersIncreaseSize(DynamicArrayCharacters *da){
  double newLength, i;
  wchar_t *newArray;
  size_t newArrayLength;

  newLength = round(da->arrayLength*3.0/2.0);
  newArray = (wchar_t*)malloc(sizeof(wchar_t) * (newLength));
  newArrayLength = newLength;

  for(i = 0.0; i < da->arrayLength; i = i + 1.0){
    newArray[(int)(i)] = da->array[(int)(i)];
  }

  free(da->array);

  da->array = newArray;
  da->arrayLength = newArrayLength;
}
_Bool DynamicArrayCharactersDecreaseSizeNecessary(DynamicArrayCharacters *da){
  _Bool needsDecrease;

  needsDecrease = false;

  if(da->length > 10.0){
    needsDecrease = da->length <= round(da->arrayLength*2.0/3.0);
  }

  return needsDecrease;
}
void DynamicArrayCharactersDecreaseSize(DynamicArrayCharacters *da){
  double newLength, i;
  wchar_t *newArray;
  size_t newArrayLength;

  newLength = round(da->arrayLength*2.0/3.0);
  newArray = (wchar_t*)malloc(sizeof(wchar_t) * (newLength));
  newArrayLength = newLength;

  for(i = 0.0; i < newLength; i = i + 1.0){
    newArray[(int)(i)] = da->array[(int)(i)];
  }

  free(da->array);

  da->array = newArray;
  da->arrayLength = newArrayLength;
}
double DynamicArrayCharactersIndex(DynamicArrayCharacters *da, double index){
  return da->array[(int)(index)];
}
double DynamicArrayCharactersLength(DynamicArrayCharacters *da){
  return da->length;
}
void DynamicArrayInsertCharacter(DynamicArrayCharacters *da, double index, wchar_t value){
  double i;

  if(da->length == da->arrayLength){
    DynamicArrayCharactersIncreaseSize(da);
  }

  for(i = da->length; i > index; i = i - 1.0){
    da->array[(int)(i)] = da->array[(int)(i - 1.0)];
  }

  da->array[(int)(index)] = value;

  da->length = da->length + 1.0;
}
_Bool DynamicArrayCharacterSet(DynamicArrayCharacters *da, double index, wchar_t value){
  _Bool success;

  if(index < da->length){
    da->array[(int)(index)] = value;
    success = true;
  }else{
    success = false;
  }

  return success;
}
void DynamicArrayRemoveCharacter(DynamicArrayCharacters *da, double index){
  double i;

  for(i = index; i < da->length - 1.0; i = i + 1.0){
    da->array[(int)(i)] = da->array[(int)(i + 1.0)];
  }

  da->length = da->length - 1.0;

  if(DynamicArrayCharactersDecreaseSizeNecessary(da)){
    DynamicArrayCharactersDecreaseSize(da);
  }
}
void FreeDynamicArrayCharacters(DynamicArrayCharacters *da){
  free(da->array);
  free(da);
}
wchar_t *DynamicArrayCharactersToArray(size_t *returnArrayLength, DynamicArrayCharacters *da){
  wchar_t *array;
  size_t arrayLength;
  double i;

  array = (wchar_t*)malloc(sizeof(wchar_t) * (da->length));
  arrayLength = da->length;

  for(i = 0.0; i < da->length; i = i + 1.0){
    array[(int)(i)] = da->array[(int)(i)];
  }

  *returnArrayLength = arrayLength;
  return array;
}
DynamicArrayCharacters *ArrayToDynamicArrayCharactersWithOptimalSize(wchar_t *array, size_t arrayLength){
  DynamicArrayCharacters *da;
  double i;
  double c, n, newCapacity;

  c = arrayLength;
  n = (log(c) - 1.0)/log(3.0/2.0);
  newCapacity = floor(n) + 1.0;

  da = CreateDynamicArrayCharactersWithInitialCapacity(newCapacity);

  for(i = 0.0; i < arrayLength; i = i + 1.0){
    da->array[(int)(i)] = array[(int)(i)];
  }

  return da;
}
DynamicArrayCharacters *ArrayToDynamicArrayCharacters(wchar_t *array, size_t arrayLength){
  DynamicArrayCharacters *da;

  da = (DynamicArrayCharacters *)malloc(sizeof(DynamicArrayCharacters));
  da->array = CopyString(&da->arrayLength, array, arrayLength);
  da->length = arrayLength;

  return da;
}
_Bool DynamicArrayCharactersEqual(DynamicArrayCharacters *a, DynamicArrayCharacters *b){
  _Bool equal;
  double i;

  equal = true;
  if(a->length == b->length){
    for(i = 0.0; i < a->length && equal; i = i + 1.0){
      if(a->array[(int)(i)] != b->array[(int)(i)]){
        equal = false;
      }
    }
  }else{
    equal = false;
  }

  return equal;
}
LinkedListCharacters *DynamicArrayCharactersToLinkedList(DynamicArrayCharacters *da){
  LinkedListCharacters *ll;
  double i;

  ll = CreateLinkedListCharacter();

  for(i = 0.0; i < da->length; i = i + 1.0){
    LinkedListAddCharacter(ll, da->array[(int)(i)]);
  }

  return ll;
}
DynamicArrayCharacters *LinkedListToDynamicArrayCharacters(LinkedListCharacters *ll){
  DynamicArrayCharacters *da;
  double i;
  LinkedListNodeCharacters *node;

  node = ll->first;

  da = (DynamicArrayCharacters *)malloc(sizeof(DynamicArrayCharacters));
  da->length = LinkedListCharactersLength(ll);

  da->array = (wchar_t*)malloc(sizeof(wchar_t) * (da->length));
  da->arrayLength = da->length;

  for(i = 0.0; i < da->length; i = i + 1.0){
    da->array[(int)(i)] = node->value;
    node = node->next;
  }

  return da;
}
_Bool *AddBoolean(size_t *returnArrayLength, _Bool *list, size_t listLength, _Bool a){
  _Bool *newlist;
  size_t newlistLength;
  double i;

  newlist = (_Bool*)malloc(sizeof(_Bool) * (listLength + 1.0));
  newlistLength = listLength + 1.0;
  for(i = 0.0; i < listLength; i = i + 1.0){
    newlist[(int)(i)] = list[(int)(i)];
  }
  newlist[(int)(listLength)] = a;
		
  free(list);
		
  *returnArrayLength = newlistLength;
  return newlist;
}
void AddBooleanRef(BooleanArrayReference *list, _Bool i){
  list->booleanArray = AddBoolean(&list->booleanArrayLength, list->booleanArray, list->booleanArrayLength, i);
}
_Bool *RemoveBoolean(size_t *returnArrayLength, _Bool *list, size_t listLength, double n){
  _Bool *newlist;
  size_t newlistLength;
  double i;

  newlist = (_Bool*)malloc(sizeof(_Bool) * (listLength - 1.0));
  newlistLength = listLength - 1.0;

  if(n >= 0.0 && n < listLength){
    for(i = 0.0; i < listLength; i = i + 1.0){
      if(i < n){
        newlist[(int)(i)] = list[(int)(i)];
      }
      if(i > n){
        newlist[(int)(i - 1.0)] = list[(int)(i)];
      }
    }

    free(list);
  }else{
    free(newlist);
  }
		
  *returnArrayLength = newlistLength;
  return newlist;
}
_Bool GetBooleanRef(BooleanArrayReference *list, double i){
  return list->booleanArray[(int)(i)];
}
void RemoveDecimalRef(BooleanArrayReference *list, double i){
  list->booleanArray = RemoveBoolean(&list->booleanArrayLength, list->booleanArray, list->booleanArrayLength, i);
}
LinkedListStrings *CreateLinkedListString(){
  LinkedListStrings *ll;

  ll = (LinkedListStrings *)malloc(sizeof(LinkedListStrings));
  ll->first = (LinkedListNodeStrings *)malloc(sizeof(LinkedListNodeStrings));
  ll->last = ll->first;
  ll->last->end = true;

  return ll;
}
void LinkedListAddString(LinkedListStrings *ll, wchar_t *value, size_t valueLength){
  ll->last->end = false;
  ll->last->value = value;
  ll->last->valueLength = valueLength;
  ll->last->next = (LinkedListNodeStrings *)malloc(sizeof(LinkedListNodeStrings));
  ll->last->next->end = true;
  ll->last = ll->last->next;
}
StringReference **LinkedListStringsToArray(size_t *returnArrayLength, LinkedListStrings *ll){
  StringReference **array;
  size_t arrayLength;
  double length, i;
  LinkedListNodeStrings *node;

  node = ll->first;

  length = LinkedListStringsLength(ll);

  array = (StringReference**)malloc(sizeof(StringReference) * length);
  arrayLength = length;

  for(i = 0.0; i < length; i = i + 1.0){
    array[(int)(i)] = (StringReference *)malloc(sizeof(StringReference));
    array[(int)(i)]->string = node->value;
    array[(int)(i)]->stringLength = node->valueLength;
    node = node->next;
  }

  *returnArrayLength = arrayLength;
  return array;
}
double LinkedListStringsLength(LinkedListStrings *ll){
  double l;
  LinkedListNodeStrings *node;

  l = 0.0;
  node = ll->first;
  for(;  !node->end ; ){
    node = node->next;
    l = l + 1.0;
  }

  return l;
}
void FreeLinkedListString(LinkedListStrings *ll){
  LinkedListNodeStrings *node, *prev;

  node = ll->first;

  for(;  !node->end ; ){
    prev = node;
    node = node->next;
    free(prev);
  }

  free(node);
  free(ll);
}
LinkedListNumbers *CreateLinkedListNumbers(){
  LinkedListNumbers *ll;

  ll = (LinkedListNumbers *)malloc(sizeof(LinkedListNumbers));
  ll->first = (LinkedListNodeNumbers *)malloc(sizeof(LinkedListNodeNumbers));
  ll->last = ll->first;
  ll->last->end = true;

  return ll;
}
LinkedListNumbers **CreateLinkedListNumbersArray(size_t *returnArrayLength, double length){
  LinkedListNumbers **lls;
  size_t llsLength;
  double i;

  lls = (LinkedListNumbers**)malloc(sizeof(LinkedListNumbers) * length);
  llsLength = length;
  for(i = 0.0; i < llsLength; i = i + 1.0){
    lls[(int)(i)] = CreateLinkedListNumbers();
  }

  *returnArrayLength = llsLength;
  return lls;
}
void LinkedListAddNumber(LinkedListNumbers *ll, double value){
  ll->last->end = false;
  ll->last->value = value;
  ll->last->next = (LinkedListNodeNumbers *)malloc(sizeof(LinkedListNodeNumbers));
  ll->last->next->end = true;
  ll->last = ll->last->next;
}
double LinkedListNumbersLength(LinkedListNumbers *ll){
  double l;
  LinkedListNodeNumbers *node;

  l = 0.0;
  node = ll->first;
  for(;  !node->end ; ){
    node = node->next;
    l = l + 1.0;
  }

  return l;
}
double LinkedListNumbersIndex(LinkedListNumbers *ll, double index){
  double i;
  LinkedListNodeNumbers *node;

  node = ll->first;
  for(i = 0.0; i < index; i = i + 1.0){
    node = node->next;
  }

  return node->value;
}
void LinkedListInsertNumber(LinkedListNumbers *ll, double index, double value){
  double i;
  LinkedListNodeNumbers *node, *tmp;

  if(index == 0.0){
    tmp = ll->first;
    ll->first = (LinkedListNodeNumbers *)malloc(sizeof(LinkedListNodeNumbers));
    ll->first->next = tmp;
    ll->first->value = value;
    ll->first->end = false;
  }else{
    node = ll->first;
    for(i = 0.0; i < index - 1.0; i = i + 1.0){
      node = node->next;
    }

    tmp = node->next;
    node->next = (LinkedListNodeNumbers *)malloc(sizeof(LinkedListNodeNumbers));
    node->next->next = tmp;
    node->next->value = value;
    node->next->end = false;
  }
}
void LinkedListSet(LinkedListNumbers *ll, double index, double value){
  double i;
  LinkedListNodeNumbers *node;

  node = ll->first;
  for(i = 0.0; i < index; i = i + 1.0){
    node = node->next;
  }

  node->next->value = value;
}
void LinkedListRemoveNumber(LinkedListNumbers *ll, double index){
  double i;
  LinkedListNodeNumbers *node, *prev;

  node = ll->first;
  prev = ll->first;

  for(i = 0.0; i < index; i = i + 1.0){
    prev = node;
    node = node->next;
  }

  if(index == 0.0){
    ll->first = prev->next;
  }
  if( !prev->next->end ){
    prev->next = prev->next->next;
  }
}
void FreeLinkedListNumbers(LinkedListNumbers *ll){
  LinkedListNodeNumbers *node, *prev;

  node = ll->first;

  for(;  !node->end ; ){
    prev = node;
    node = node->next;
    free(prev);
  }

  free(node);
}
void FreeLinkedListNumbersArray(LinkedListNumbers **lls, size_t llsLength){
  double i;

  for(i = 0.0; i < llsLength; i = i + 1.0){
    FreeLinkedListNumbers(lls[(int)(i)]);
  }
  free(lls);
}
double *LinkedListNumbersToArray(size_t *returnArrayLength, LinkedListNumbers *ll){
  double *array;
  size_t arrayLength;
  double length, i;
  LinkedListNodeNumbers *node;

  node = ll->first;

  length = LinkedListNumbersLength(ll);

  array = (double*)malloc(sizeof(double) * (length));
  arrayLength = length;

  for(i = 0.0; i < length; i = i + 1.0){
    array[(int)(i)] = node->value;
    node = node->next;
  }

  *returnArrayLength = arrayLength;
  return array;
}
LinkedListNumbers *ArrayToLinkedListNumbers(double *array, size_t arrayLength){
  LinkedListNumbers *ll;
  double i;

  ll = CreateLinkedListNumbers();

  for(i = 0.0; i < arrayLength; i = i + 1.0){
    LinkedListAddNumber(ll, array[(int)(i)]);
  }

  return ll;
}
_Bool LinkedListNumbersEqual(LinkedListNumbers *a, LinkedListNumbers *b){
  _Bool equal, done;
  LinkedListNodeNumbers *an, *bn;

  an = a->first;
  bn = b->first;

  equal = true;
  done = false;
  for(; equal &&  !done ; ){
    if(an->end == bn->end){
      if(an->end){
        done = true;
      }else if(an->value == bn->value){
        an = an->next;
        bn = bn->next;
      }else{
        equal = false;
      }
    }else{
      equal = false;
    }
  }

  return equal;
}
LinkedListCharacters *CreateLinkedListCharacter(){
  LinkedListCharacters *ll;

  ll = (LinkedListCharacters *)malloc(sizeof(LinkedListCharacters));
  ll->first = (LinkedListNodeCharacters *)malloc(sizeof(LinkedListNodeCharacters));
  ll->last = ll->first;
  ll->last->end = true;

  return ll;
}
void LinkedListAddCharacter(LinkedListCharacters *ll, wchar_t value){
  ll->last->end = false;
  ll->last->value = value;
  ll->last->next = (LinkedListNodeCharacters *)malloc(sizeof(LinkedListNodeCharacters));
  ll->last->next->end = true;
  ll->last = ll->last->next;
}
wchar_t *LinkedListCharactersToArray(size_t *returnArrayLength, LinkedListCharacters *ll){
  wchar_t *array;
  size_t arrayLength;
  double length, i;
  LinkedListNodeCharacters *node;

  node = ll->first;

  length = LinkedListCharactersLength(ll);

  array = (wchar_t*)malloc(sizeof(wchar_t) * (length));
  arrayLength = length;

  for(i = 0.0; i < length; i = i + 1.0){
    array[(int)(i)] = node->value;
    node = node->next;
  }

  *returnArrayLength = arrayLength;
  return array;
}
double LinkedListCharactersLength(LinkedListCharacters *ll){
  double l;
  LinkedListNodeCharacters *node;

  l = 0.0;
  node = ll->first;
  for(;  !node->end ; ){
    node = node->next;
    l = l + 1.0;
  }

  return l;
}
void FreeLinkedListCharacter(LinkedListCharacters *ll){
  LinkedListNodeCharacters *node, *prev;

  node = ll->first;

  for(;  !node->end ; ){
    prev = node;
    node = node->next;
    free(prev);
  }

  free(node);
}
void LinkedListCharactersAddString(LinkedListCharacters *ll, wchar_t *str, size_t strLength){
  double i;

  for(i = 0.0; i < strLength; i = i + 1.0){
    LinkedListAddCharacter(ll, str[(int)(i)]);
  }
}
DynamicArrayNumbers *CreateDynamicArrayNumbers(){
  DynamicArrayNumbers *da;

  da = (DynamicArrayNumbers *)malloc(sizeof(DynamicArrayNumbers));
  da->array = (double*)malloc(sizeof(double) * (10.0));
  da->arrayLength = 10.0;
  da->length = 0.0;

  return da;
}
DynamicArrayNumbers *CreateDynamicArrayNumbersWithInitialCapacity(double capacity){
  DynamicArrayNumbers *da;

  da = (DynamicArrayNumbers *)malloc(sizeof(DynamicArrayNumbers));
  da->array = (double*)malloc(sizeof(double) * (capacity));
  da->arrayLength = capacity;
  da->length = 0.0;

  return da;
}
void DynamicArrayAddNumber(DynamicArrayNumbers *da, double value){
  if(da->length == da->arrayLength){
    DynamicArrayNumbersIncreaseSize(da);
  }

  da->array[(int)(da->length)] = value;
  da->length = da->length + 1.0;
}
void DynamicArrayNumbersIncreaseSize(DynamicArrayNumbers *da){
  double newLength, i;
  double *newArray;
  size_t newArrayLength;

  newLength = round(da->arrayLength*3.0/2.0);
  newArray = (double*)malloc(sizeof(double) * (newLength));
  newArrayLength = newLength;

  for(i = 0.0; i < da->arrayLength; i = i + 1.0){
    newArray[(int)(i)] = da->array[(int)(i)];
  }

  free(da->array);

  da->array = newArray;
  da->arrayLength = newArrayLength;
}
_Bool DynamicArrayNumbersDecreaseSizeNecessary(DynamicArrayNumbers *da){
  _Bool needsDecrease;

  needsDecrease = false;

  if(da->length > 10.0){
    needsDecrease = da->length <= round(da->arrayLength*2.0/3.0);
  }

  return needsDecrease;
}
void DynamicArrayNumbersDecreaseSize(DynamicArrayNumbers *da){
  double newLength, i;
  double *newArray;
  size_t newArrayLength;

  newLength = round(da->arrayLength*2.0/3.0);
  newArray = (double*)malloc(sizeof(double) * (newLength));
  newArrayLength = newLength;

  for(i = 0.0; i < newLength; i = i + 1.0){
    newArray[(int)(i)] = da->array[(int)(i)];
  }

  free(da->array);

  da->array = newArray;
  da->arrayLength = newArrayLength;
}
double DynamicArrayNumbersIndex(DynamicArrayNumbers *da, double index){
  return da->array[(int)(index)];
}
double DynamicArrayNumbersLength(DynamicArrayNumbers *da){
  return da->length;
}
void DynamicArrayInsertNumber(DynamicArrayNumbers *da, double index, double value){
  double i;

  if(da->length == da->arrayLength){
    DynamicArrayNumbersIncreaseSize(da);
  }

  for(i = da->length; i > index; i = i - 1.0){
    da->array[(int)(i)] = da->array[(int)(i - 1.0)];
  }

  da->array[(int)(index)] = value;

  da->length = da->length + 1.0;
}
_Bool DynamicArrayNumberSet(DynamicArrayNumbers *da, double index, double value){
  _Bool success;

  if(index < da->length){
    da->array[(int)(index)] = value;
    success = true;
  }else{
    success = false;
  }

  return success;
}
void DynamicArrayRemoveNumber(DynamicArrayNumbers *da, double index){
  double i;

  for(i = index; i < da->length - 1.0; i = i + 1.0){
    da->array[(int)(i)] = da->array[(int)(i + 1.0)];
  }

  da->length = da->length - 1.0;

  if(DynamicArrayNumbersDecreaseSizeNecessary(da)){
    DynamicArrayNumbersDecreaseSize(da);
  }
}
void FreeDynamicArrayNumbers(DynamicArrayNumbers *da){
  free(da->array);
  free(da);
}
double *DynamicArrayNumbersToArray(size_t *returnArrayLength, DynamicArrayNumbers *da){
  double *array;
  size_t arrayLength;
  double i;

  array = (double*)malloc(sizeof(double) * (da->length));
  arrayLength = da->length;

  for(i = 0.0; i < da->length; i = i + 1.0){
    array[(int)(i)] = da->array[(int)(i)];
  }

  *returnArrayLength = arrayLength;
  return array;
}
DynamicArrayNumbers *ArrayToDynamicArrayNumbersWithOptimalSize(double *array, size_t arrayLength){
  DynamicArrayNumbers *da;
  double i;
  double c, n, newCapacity;

  /*
         c = 10*(3/2)^n
         log(c) = log(10*(3/2)^n)
         log(c) = log(10) + log((3/2)^n)
         log(c) = 1 + log((3/2)^n)
         log(c) - 1 = log((3/2)^n)
         log(c) - 1 = n*log(3/2)
         n = (log(c) - 1)/log(3/2)
         */
  c = arrayLength;
  n = (log(c) - 1.0)/log(3.0/2.0);
  newCapacity = floor(n) + 1.0;

  da = CreateDynamicArrayNumbersWithInitialCapacity(newCapacity);

  for(i = 0.0; i < arrayLength; i = i + 1.0){
    da->array[(int)(i)] = array[(int)(i)];
  }

  return da;
}
DynamicArrayNumbers *ArrayToDynamicArrayNumbers(double *array, size_t arrayLength){
  DynamicArrayNumbers *da;

  da = (DynamicArrayNumbers *)malloc(sizeof(DynamicArrayNumbers));
  da->array = CopyNumberArray(&da->arrayLength, array, arrayLength);
  da->length = arrayLength;

  return da;
}
_Bool DynamicArrayNumbersEqual(DynamicArrayNumbers *a, DynamicArrayNumbers *b){
  _Bool equal;
  double i;

  equal = true;
  if(a->length == b->length){
    for(i = 0.0; i < a->length && equal; i = i + 1.0){
      if(a->array[(int)(i)] != b->array[(int)(i)]){
        equal = false;
      }
    }
  }else{
    equal = false;
  }

  return equal;
}
LinkedListNumbers *DynamicArrayNumbersToLinkedList(DynamicArrayNumbers *da){
  LinkedListNumbers *ll;
  double i;

  ll = CreateLinkedListNumbers();

  for(i = 0.0; i < da->length; i = i + 1.0){
    LinkedListAddNumber(ll, da->array[(int)(i)]);
  }

  return ll;
}
DynamicArrayNumbers *LinkedListToDynamicArrayNumbers(LinkedListNumbers *ll){
  DynamicArrayNumbers *da;
  double i;
  LinkedListNodeNumbers *node;

  node = ll->first;

  da = (DynamicArrayNumbers *)malloc(sizeof(DynamicArrayNumbers));
  da->length = LinkedListNumbersLength(ll);

  da->array = (double*)malloc(sizeof(double) * (da->length));
  da->arrayLength = da->length;

  for(i = 0.0; i < da->length; i = i + 1.0){
    da->array[(int)(i)] = node->value;
    node = node->next;
  }

  return da;
}
double DynamicArrayNumbersIndexOf(DynamicArrayNumbers *arr, double n, BooleanReference *foundReference){
  _Bool found;
  double i;

  found = false;
  for(i = 0.0; i < arr->length &&  !found ; i = i + 1.0){
    if(arr->array[(int)(i)] == n){
      found = true;
    }
  }
  if( !found ){
    i =  -1.0;
  }else{
    i = i - 1.0;
  }

  foundReference->booleanValue = found;

  return i;
}
_Bool DynamicArrayNumbersIsInArray(DynamicArrayNumbers *arr, double n){
  _Bool found;
  double i;

  found = false;
  for(i = 0.0; i < arr->length &&  !found ; i = i + 1.0){
    if(arr->array[(int)(i)] == n){
      found = true;
    }
  }

  return found;
}
wchar_t *AddCharacter(size_t *returnArrayLength, wchar_t *list, size_t listLength, wchar_t a){
  wchar_t *newlist;
  size_t newlistLength;
  double i;

  newlist = (wchar_t*)malloc(sizeof(wchar_t) * (listLength + 1.0));
  newlistLength = listLength + 1.0;
  for(i = 0.0; i < listLength; i = i + 1.0){
    newlist[(int)(i)] = list[(int)(i)];
  }
  newlist[(int)(listLength)] = a;
		
  free(list);
		
  *returnArrayLength = newlistLength;
  return newlist;
}
void AddCharacterRef(StringReference *list, wchar_t i){
  list->string = AddCharacter(&list->stringLength, list->string, list->stringLength, i);
}
wchar_t *RemoveCharacter(size_t *returnArrayLength, wchar_t *list, size_t listLength, double n){
  wchar_t *newlist;
  size_t newlistLength;
  double i;

  newlist = (wchar_t*)malloc(sizeof(wchar_t) * (listLength - 1.0));
  newlistLength = listLength - 1.0;

  if(n >= 0.0 && n < listLength){
    for(i = 0.0; i < listLength; i = i + 1.0){
      if(i < n){
        newlist[(int)(i)] = list[(int)(i)];
      }
      if(i > n){
        newlist[(int)(i - 1.0)] = list[(int)(i)];
      }
    }

    free(list);
  }else{
    free(newlist);
  }

  *returnArrayLength = newlistLength;
  return newlist;
}
wchar_t GetCharacterRef(StringReference *list, double i){
  return list->string[(int)(i)];
}
void RemoveCharacterRef(StringReference *list, double i){
  list->string = RemoveCharacter(&list->stringLength, list->string, list->stringLength, i);
}
double Negate(double x){
  return  -x;
}
double Positive(double x){
  return  +x;
}
double Factorial(double x){
  double i, f;

  f = 1.0;

  for(i = 2.0; i <= x; i = i + 1.0){
    f = f*i;
  }

  return f;
}
double Round(double x){
  return floor(x + 0.5);
}
double BankersRound(double x){
  double r;

  if(Absolute(x - Truncate(x)) == 0.5){
    if( !DivisibleBy(Round(x), 2.0) ){
      r = Round(x) - 1.0;
    }else{
      r = Round(x);
    }
  }else{
    r = Round(x);
  }

  return r;
}
double Ceil(double x){
  return ceil(x);
}
double Floor(double x){
  return floor(x);
}
double Truncate(double x){
  double t;

  if(x >= 0.0){
    t = floor(x);
  }else{
    t = ceil(x);
  }

  return t;
}
double Absolute(double x){
  return fabs(x);
}
double Logarithm(double x){
  return log10(x);
}
double NaturalLogarithm(double x){
  return log(x);
}
double Sin(double x){
  return sin(x);
}
double Cos(double x){
  return cos(x);
}
double Tan(double x){
  return tan(x);
}
double Asin(double x){
  return asin(x);
}
double Acos(double x){
  return acos(x);
}
double Atan(double x){
  return atan(x);
}
double Atan2(double y, double x){
  double a;

  /* Atan2 is an invalid operation when x = 0 and y = 0, but this method does not return errors. */
  a = 0.0;

  if(x > 0.0){
    a = Atan(y/x);
  }else if(x < 0.0 && y >= 0.0){
    a = Atan(y/x) + M_PI;
  }else if(x < 0.0 && y < 0.0){
    a = Atan(y/x) - M_PI;
  }else if(x == 0.0 && y > 0.0){
    a = M_PI/2.0;
  }else if(x == 0.0 && y < 0.0){
    a =  -M_PI/2.0;
  }

  return a;
}
double Squareroot(double x){
  return sqrt(x);
}
double Exp(double x){
  return exp(x);
}
_Bool DivisibleBy(double a, double b){
  return ((fmod(a, b)) == 0.0);
}
double Combinations(double n, double k){
  double i, j, c;

  c = 1.0;
  j = 1.0;
  i = n - k + 1.0;

  for(; i <= n; ){
    c = c*i;
    c = c/j;

    i = i + 1.0;
    j = j + 1.0;
  }

  return c;
}
double Permutations(double n, double k){
  double i, c;

  c = 1.0;

  for(i = n - k + 1.0; i <= n; i = i + 1.0){
    c = c*i;
  }

  return c;
}
_Bool EpsilonCompare(double a, double b, double epsilon){
  return fabs(a - b) < epsilon;
}
double GreatestCommonDivisor(double a, double b){
  double t;

  for(; b != 0.0; ){
    t = b;
    b = fmod(a, b);
    a = t;
  }

  return a;
}
double GCDWithSubtraction(double a, double b){
  double g;

  if(a == 0.0){
    g = b;
  }else{
    for(; b != 0.0; ){
      if(a > b){
        a = a - b;
      }else{
        b = b - a;
      }
    }

    g = a;
  }

  return g;
}
_Bool IsInteger(double a){
  return (a - floor(a)) == 0.0;
}
_Bool GreatestCommonDivisorWithCheck(double a, double b, NumberReference *gcdReference){
  _Bool success;
  double gcd;

  if(IsInteger(a) && IsInteger(b)){
    gcd = GreatestCommonDivisor(a, b);
    gcdReference->numberValue = gcd;
    success = true;
  }else{
    success = false;
  }

  return success;
}
double LeastCommonMultiple(double a, double b){
  double lcm;

  if(a > 0.0 && b > 0.0){
    lcm = fabs(a*b)/GreatestCommonDivisor(a, b);
  }else{
    lcm = 0.0;
  }

  return lcm;
}
double Sign(double a){
  double s;

  if(a > 0.0){
    s = 1.0;
  }else if(a < 0.0){
    s =  -1.0;
  }else{
    s = 0.0;
  }

  return s;
}
double Max(double a, double b){
  return fmax(a, b);
}
double Min(double a, double b){
  return fmin(a, b);
}
double Power(double a, double b){
  return pow(a, b);
}
double Gamma(double x){
  return LanczosApproximation(x);
}
double LogGamma(double x){
  return log(Gamma(x));
}
double LanczosApproximation(double z){
  double *p;
  size_t pLength;
  double i, y, t, x;

  p = (double*)malloc(sizeof(double) * (8.0));
  pLength = 8.0;
  p[0] = 676.5203681218851;
  p[1] =  -1259.1392167224028;
  p[2] = 771.32342877765313;
  p[3] =  -176.61502916214059;
  p[4] = 12.507343278686905;
  p[5] =  -0.13857109526572012;
  p[6] = 9.9843695780195716e-6;
  p[7] = 1.5056327351493116e-7;

  if(z < 0.5){
    y = M_PI/(sin(M_PI*z)*LanczosApproximation(1.0 - z));
  }else{
    z = z - 1.0;
    x = 0.99999999999980993;
    for(i = 0.0; i < pLength; i = i + 1.0){
      x = x + p[(int)(i)]/(z + i + 1.0);
    }
    t = z + pLength - 0.5;
    y = sqrt(2.0*M_PI)*pow(t, z + 0.5)*exp( -t)*x;
  }

  return y;
}
double Beta(double x, double y){
  return Gamma(x)*Gamma(y)/Gamma(x + y);
}
double Sinh(double x){
  return (exp(x) - exp( -x))/2.0;
}
double Cosh(double x){
  return (exp(x) + exp( -x))/2.0;
}
double Tanh(double x){
  return Sinh(x)/Cosh(x);
}
double Cot(double x){
  return 1.0/tan(x);
}
double Sec(double x){
  return 1.0/cos(x);
}
double Csc(double x){
  return 1.0/sin(x);
}
double Coth(double x){
  return Cosh(x)/Sinh(x);
}
double Sech(double x){
  return 1.0/Cosh(x);
}
double Csch(double x){
  return 1.0/Sinh(x);
}
double Error(double x){
  double y, t, tau, c1, c2, c3, c4, c5, c6, c7, c8, c9, c10;

  if(x == 0.0){
    y = 0.0;
  }else if(x < 0.0){
    y =  -Error( -x);
  }else{
    c1 =  -1.26551223;
    c2 =  +1.00002368;
    c3 =  +0.37409196;
    c4 =  +0.09678418;
    c5 =  -0.18628806;
    c6 =  +0.27886807;
    c7 =  -1.13520398;
    c8 =  +1.48851587;
    c9 =  -0.82215223;
    c10 =  +0.17087277;

    t = 1.0/(1.0 + 0.5*fabs(x));

    tau = t*exp( -pow(x, 2.0) + c1 + t*(c2 + t*(c3 + t*(c4 + t*(c5 + t*(c6 + t*(c7 + t*(c8 + t*(c9 + t*c10)))))))));

    y = 1.0 - tau;
  }

  return y;
}
double ErrorInverse(double x){
  double y, a, t;

  a = (8.0*(M_PI - 3.0))/(3.0*M_PI*(4.0 - M_PI));

  t = 2.0/(M_PI*a) + log(1.0 - pow(x, 2.0))/2.0;
  y = Sign(x)*sqrt(sqrt(pow(t, 2.0) - log(1.0 - pow(x, 2.0))/a) - t);

  return y;
}
double FallingFactorial(double x, double n){
  double k, y;

  y = 1.0;

  for(k = 0.0; k <= n - 1.0; k = k + 1.0){
    y = y*(x - k);
  }

  return y;
}
double RisingFactorial(double x, double n){
  double k, y;

  y = 1.0;

  for(k = 0.0; k <= n - 1.0; k = k + 1.0){
    y = y*(x + k);
  }

  return y;
}
double Hypergeometric(double a, double b, double c, double z, double maxIterations, double precision){
  double y;

  if(fabs(z) >= 0.5){
    y = pow(1.0 - z,  -a)*HypergeometricDirect(a, c - b, c, z/(z - 1.0), maxIterations, precision);
  }else{
    y = HypergeometricDirect(a, b, c, z, maxIterations, precision);
  }

  return y;
}
double HypergeometricDirect(double a, double b, double c, double z, double maxIterations, double precision){
  double y, yp, n;
  _Bool done;

  y = 0.0;
  done = false;

  for(n = 0.0; n < maxIterations &&  !done ; n = n + 1.0){
    yp = RisingFactorial(a, n)*RisingFactorial(b, n)/RisingFactorial(c, n)*pow(z, n)/Factorial(n);
    if(fabs(yp) < precision){
      done = true;
    }
    y = y + yp;
  }

  return y;
}
double BernouilliNumber(double n){
  return AkiyamaTanigawaAlgorithm(n);
}
double AkiyamaTanigawaAlgorithm(double n){
  double m, j, B;
  double *A;
  size_t ALength;

  A = (double*)malloc(sizeof(double) * (n + 1.0));
  ALength = n + 1.0;

  for(m = 0.0; m <= n; m = m + 1.0){
    A[(int)(m)] = 1.0/(m + 1.0);
    for(j = m; j >= 1.0; j = j - 1.0){
      A[(int)(j - 1.0)] = j*(A[(int)(j - 1.0)] - A[(int)(j)]);
    }
  }

  B = A[0];

  free(A);

  return B;
}
double *StringToNumberArray(size_t *returnArrayLength, wchar_t *string, size_t stringLength){
  double i;
  double *array;
  size_t arrayLength;

  array = (double*)malloc(sizeof(double) * (stringLength));
  arrayLength = stringLength;

  for(i = 0.0; i < stringLength; i = i + 1.0){
    array[(int)(i)] = string[(int)(i)];
  }
  *returnArrayLength = arrayLength;
  return array;
}
wchar_t *NumberArrayToString(size_t *returnArrayLength, double *array, size_t arrayLength){
  double i;
  wchar_t *string;
  size_t stringLength;

  string = (wchar_t*)malloc(sizeof(wchar_t) * (arrayLength));
  stringLength = arrayLength;

  for(i = 0.0; i < arrayLength; i = i + 1.0){
    string[(int)(i)] = array[(int)(i)];
  }
  *returnArrayLength = stringLength;
  return string;
}
_Bool NumberArraysEqual(double *a, size_t aLength, double *b, size_t bLength){
  _Bool equal;
  double i;

  equal = true;
  if(aLength == bLength){
    for(i = 0.0; i < aLength && equal; i = i + 1.0){
      if(a[(int)(i)] != b[(int)(i)]){
        equal = false;
      }
    }
  }else{
    equal = false;
  }

  return equal;
}
_Bool BooleanArraysEqual(_Bool *a, size_t aLength, _Bool *b, size_t bLength){
  _Bool equal;
  double i;

  equal = true;
  if(aLength == bLength){
    for(i = 0.0; i < aLength && equal; i = i + 1.0){
      if(a[(int)(i)] != b[(int)(i)]){
        equal = false;
      }
    }
  }else{
    equal = false;
  }

  return equal;
}
_Bool StringsEqual(wchar_t *a, size_t aLength, wchar_t *b, size_t bLength){
  _Bool equal;
  double i;

  equal = true;
  if(aLength == bLength){
    for(i = 0.0; i < aLength && equal; i = i + 1.0){
      if(a[(int)(i)] != b[(int)(i)]){
        equal = false;
      }
    }
  }else{
    equal = false;
  }

  return equal;
}
void FillNumberArray(double *a, size_t aLength, double value){
  double i;

  for(i = 0.0; i < aLength; i = i + 1.0){
    a[(int)(i)] = value;
  }
}
void FillString(wchar_t *a, size_t aLength, wchar_t value){
  double i;

  for(i = 0.0; i < aLength; i = i + 1.0){
    a[(int)(i)] = value;
  }
}
void FillBooleanArray(_Bool *a, size_t aLength, _Bool value){
  double i;

  for(i = 0.0; i < aLength; i = i + 1.0){
    a[(int)(i)] = value;
  }
}
_Bool FillNumberArrayRange(double *a, size_t aLength, double value, double from, double to){
  double i, length;
  _Bool success;

  if(from >= 0.0 && from <= aLength && to >= 0.0 && to <= aLength && from <= to){
    length = to - from;
    for(i = 0.0; i < length; i = i + 1.0){
      a[(int)(from + i)] = value;
    }

    success = true;
  }else{
    success = false;
  }

  return success;
}
_Bool FillBooleanArrayRange(_Bool *a, size_t aLength, _Bool value, double from, double to){
  double i, length;
  _Bool success;

  if(from >= 0.0 && from <= aLength && to >= 0.0 && to <= aLength && from <= to){
    length = to - from;
    for(i = 0.0; i < length; i = i + 1.0){
      a[(int)(from + i)] = value;
    }

    success = true;
  }else{
    success = false;
  }

  return success;
}
_Bool FillStringRange(wchar_t *a, size_t aLength, wchar_t value, double from, double to){
  double i, length;
  _Bool success;

  if(from >= 0.0 && from <= aLength && to >= 0.0 && to <= aLength && from <= to){
    length = to - from;
    for(i = 0.0; i < length; i = i + 1.0){
      a[(int)(from + i)] = value;
    }

    success = true;
  }else{
    success = false;
  }

  return success;
}
double *CopyNumberArray(size_t *returnArrayLength, double *a, size_t aLength){
  double i;
  double *n;
  size_t nLength;

  n = (double*)malloc(sizeof(double) * (aLength));
  nLength = aLength;

  for(i = 0.0; i < aLength; i = i + 1.0){
    n[(int)(i)] = a[(int)(i)];
  }

  *returnArrayLength = nLength;
  return n;
}
_Bool *CopyBooleanArray(size_t *returnArrayLength, _Bool *a, size_t aLength){
  double i;
  _Bool *n;
  size_t nLength;

  n = (_Bool*)malloc(sizeof(_Bool) * (aLength));
  nLength = aLength;

  for(i = 0.0; i < aLength; i = i + 1.0){
    n[(int)(i)] = a[(int)(i)];
  }

  *returnArrayLength = nLength;
  return n;
}
wchar_t *CopyString(size_t *returnArrayLength, wchar_t *a, size_t aLength){
  double i;
  wchar_t *n;
  size_t nLength;

  n = (wchar_t*)malloc(sizeof(wchar_t) * (aLength));
  nLength = aLength;

  for(i = 0.0; i < aLength; i = i + 1.0){
    n[(int)(i)] = a[(int)(i)];
  }

  *returnArrayLength = nLength;
  return n;
}
_Bool CopyNumberArrayRange(double *a, size_t aLength, double from, double to, NumberArrayReference *copyReference){
  double i, length;
  double *n;
  size_t nLength;
  _Bool success;

  if(from >= 0.0 && from <= aLength && to >= 0.0 && to <= aLength && from <= to){
    length = to - from;
    n = (double*)malloc(sizeof(double) * (length));
    nLength = length;

    for(i = 0.0; i < length; i = i + 1.0){
      n[(int)(i)] = a[(int)(from + i)];
    }

    copyReference->numberArray = n;
    copyReference->numberArrayLength = nLength;
    success = true;
  }else{
    success = false;
  }

  return success;
}
_Bool CopyBooleanArrayRange(_Bool *a, size_t aLength, double from, double to, BooleanArrayReference *copyReference){
  double i, length;
  _Bool *n;
  size_t nLength;
  _Bool success;

  if(from >= 0.0 && from <= aLength && to >= 0.0 && to <= aLength && from <= to){
    length = to - from;
    n = (_Bool*)malloc(sizeof(_Bool) * (length));
    nLength = length;

    for(i = 0.0; i < length; i = i + 1.0){
      n[(int)(i)] = a[(int)(from + i)];
    }

    copyReference->booleanArray = n;
    copyReference->booleanArrayLength = nLength;
    success = true;
  }else{
    success = false;
  }

  return success;
}
_Bool CopyStringRange(wchar_t *a, size_t aLength, double from, double to, StringReference *copyReference){
  double i, length;
  wchar_t *n;
  size_t nLength;
  _Bool success;

  if(from >= 0.0 && from <= aLength && to >= 0.0 && to <= aLength && from <= to){
    length = to - from;
    n = (wchar_t*)malloc(sizeof(wchar_t) * (length));
    nLength = length;

    for(i = 0.0; i < length; i = i + 1.0){
      n[(int)(i)] = a[(int)(from + i)];
    }

    copyReference->string = n;
    copyReference->stringLength = nLength;
    success = true;
  }else{
    success = false;
  }

  return success;
}
_Bool IsLastElement(double length, double index){
  return index + 1.0 == length;
}
double *CreateNumberArray(size_t *returnArrayLength, double length, double value){
  double *array;
  size_t arrayLength;

  array = (double*)malloc(sizeof(double) * (length));
  arrayLength = length;
  FillNumberArray(array, arrayLength, value);

  *returnArrayLength = arrayLength;
  return array;
}
_Bool *CreateBooleanArray(size_t *returnArrayLength, double length, _Bool value){
  _Bool *array;
  size_t arrayLength;

  array = (_Bool*)malloc(sizeof(_Bool) * (length));
  arrayLength = length;
  FillBooleanArray(array, arrayLength, value);

  *returnArrayLength = arrayLength;
  return array;
}
wchar_t *CreateString(size_t *returnArrayLength, double length, wchar_t value){
  wchar_t *array;
  size_t arrayLength;

  array = (wchar_t*)malloc(sizeof(wchar_t) * (length));
  arrayLength = length;
  FillString(array, arrayLength, value);

  *returnArrayLength = arrayLength;
  return array;
}
void SwapElementsOfNumberArray(double *A, size_t ALength, double ai, double bi){
  double tmp;

  tmp = A[(int)(ai)];
  A[(int)(ai)] = A[(int)(bi)];
  A[(int)(bi)] = tmp;
}
void SwapElementsOfStringArray(StringArrayReference *A, double ai, double bi){
  StringReference *tmp;

  tmp = A->stringArray[(int)(ai)];
  A->stringArray[(int)(ai)] = A->stringArray[(int)(bi)];
  A->stringArray[(int)(bi)] = tmp;
}
void ReverseNumberArray(double *array, size_t arrayLength){
  double i;

  for(i = 0.0; i < arrayLength/2.0; i = i + 1.0){
    SwapElementsOfNumberArray(array, arrayLength, i, arrayLength - i - 1.0);
  }
}
wchar_t charToLowerCase(wchar_t character){
  wchar_t toReturn;

  toReturn = character;
  if(character == 'A'){
    toReturn = 'a';
  }else if(character == 'B'){
    toReturn = 'b';
  }else if(character == 'C'){
    toReturn = 'c';
  }else if(character == 'D'){
    toReturn = 'd';
  }else if(character == 'E'){
    toReturn = 'e';
  }else if(character == 'F'){
    toReturn = 'f';
  }else if(character == 'G'){
    toReturn = 'g';
  }else if(character == 'H'){
    toReturn = 'h';
  }else if(character == 'I'){
    toReturn = 'i';
  }else if(character == 'J'){
    toReturn = 'j';
  }else if(character == 'K'){
    toReturn = 'k';
  }else if(character == 'L'){
    toReturn = 'l';
  }else if(character == 'M'){
    toReturn = 'm';
  }else if(character == 'N'){
    toReturn = 'n';
  }else if(character == 'O'){
    toReturn = 'o';
  }else if(character == 'P'){
    toReturn = 'p';
  }else if(character == 'Q'){
    toReturn = 'q';
  }else if(character == 'R'){
    toReturn = 'r';
  }else if(character == 'S'){
    toReturn = 's';
  }else if(character == 'T'){
    toReturn = 't';
  }else if(character == 'U'){
    toReturn = 'u';
  }else if(character == 'V'){
    toReturn = 'v';
  }else if(character == 'W'){
    toReturn = 'w';
  }else if(character == 'X'){
    toReturn = 'x';
  }else if(character == 'Y'){
    toReturn = 'y';
  }else if(character == 'Z'){
    toReturn = 'z';
  }

  return toReturn;
}
wchar_t charToUpperCase(wchar_t character){
  wchar_t toReturn;

  toReturn = character;
  if(character == 'a'){
    toReturn = 'A';
  }else if(character == 'b'){
    toReturn = 'B';
  }else if(character == 'c'){
    toReturn = 'C';
  }else if(character == 'd'){
    toReturn = 'D';
  }else if(character == 'e'){
    toReturn = 'E';
  }else if(character == 'f'){
    toReturn = 'F';
  }else if(character == 'g'){
    toReturn = 'G';
  }else if(character == 'h'){
    toReturn = 'H';
  }else if(character == 'i'){
    toReturn = 'I';
  }else if(character == 'j'){
    toReturn = 'J';
  }else if(character == 'k'){
    toReturn = 'K';
  }else if(character == 'l'){
    toReturn = 'L';
  }else if(character == 'm'){
    toReturn = 'M';
  }else if(character == 'n'){
    toReturn = 'N';
  }else if(character == 'o'){
    toReturn = 'O';
  }else if(character == 'p'){
    toReturn = 'P';
  }else if(character == 'q'){
    toReturn = 'Q';
  }else if(character == 'r'){
    toReturn = 'R';
  }else if(character == 's'){
    toReturn = 'S';
  }else if(character == 't'){
    toReturn = 'T';
  }else if(character == 'u'){
    toReturn = 'U';
  }else if(character == 'v'){
    toReturn = 'V';
  }else if(character == 'w'){
    toReturn = 'W';
  }else if(character == 'x'){
    toReturn = 'X';
  }else if(character == 'y'){
    toReturn = 'Y';
  }else if(character == 'z'){
    toReturn = 'Z';
  }

  return toReturn;
}
_Bool charIsUpperCase(wchar_t character){
  _Bool isUpper;

  isUpper = true;
  if(character == 'A'){
  }else if(character == 'B'){
  }else if(character == 'C'){
  }else if(character == 'D'){
  }else if(character == 'E'){
  }else if(character == 'F'){
  }else if(character == 'G'){
  }else if(character == 'H'){
  }else if(character == 'I'){
  }else if(character == 'J'){
  }else if(character == 'K'){
  }else if(character == 'L'){
  }else if(character == 'M'){
  }else if(character == 'N'){
  }else if(character == 'O'){
  }else if(character == 'P'){
  }else if(character == 'Q'){
  }else if(character == 'R'){
  }else if(character == 'S'){
  }else if(character == 'T'){
  }else if(character == 'U'){
  }else if(character == 'V'){
  }else if(character == 'W'){
  }else if(character == 'X'){
  }else if(character == 'Y'){
  }else if(character == 'Z'){
  }else{
    isUpper = false;
  }

  return isUpper;
}
_Bool charIsLowerCase(wchar_t character){
  _Bool isLower;

  isLower = true;
  if(character == 'a'){
  }else if(character == 'b'){
  }else if(character == 'c'){
  }else if(character == 'd'){
  }else if(character == 'e'){
  }else if(character == 'f'){
  }else if(character == 'g'){
  }else if(character == 'h'){
  }else if(character == 'i'){
  }else if(character == 'j'){
  }else if(character == 'k'){
  }else if(character == 'l'){
  }else if(character == 'm'){
  }else if(character == 'n'){
  }else if(character == 'o'){
  }else if(character == 'p'){
  }else if(character == 'q'){
  }else if(character == 'r'){
  }else if(character == 's'){
  }else if(character == 't'){
  }else if(character == 'u'){
  }else if(character == 'v'){
  }else if(character == 'w'){
  }else if(character == 'x'){
  }else if(character == 'y'){
  }else if(character == 'z'){
  }else{
    isLower = false;
  }

  return isLower;
}
_Bool charIsLetter(wchar_t character){
  return charIsUpperCase(character) || charIsLowerCase(character);
}
_Bool charIsNumber(wchar_t character){
  _Bool isNumberx;

  isNumberx = true;
  if(character == '0'){
  }else if(character == '1'){
  }else if(character == '2'){
  }else if(character == '3'){
  }else if(character == '4'){
  }else if(character == '5'){
  }else if(character == '6'){
  }else if(character == '7'){
  }else if(character == '8'){
  }else if(character == '9'){
  }else{
    isNumberx = false;
  }

  return isNumberx;
}
_Bool charIsWhiteSpace(wchar_t character){
  _Bool isWhiteSpacex;

  isWhiteSpacex = true;
  if(character == ' '){
  }else if(character == '\t'){
  }else if(character == '\n'){
  }else if(character == '\r'){
  }else{
    isWhiteSpacex = false;
  }

  return isWhiteSpacex;
}
_Bool charIsSymbol(wchar_t character){
  _Bool isSymbolx;

  isSymbolx = true;
  if(character == '!'){
  }else if(character == '\"'){
  }else if(character == '#'){
  }else if(character == '$'){
  }else if(character == '%'){
  }else if(character == '&'){
  }else if(character == '\''){
  }else if(character == '('){
  }else if(character == ')'){
  }else if(character == '*'){
  }else if(character == '+'){
  }else if(character == ','){
  }else if(character == '-'){
  }else if(character == '.'){
  }else if(character == '/'){
  }else if(character == ':'){
  }else if(character == ';'){
  }else if(character == '<'){
  }else if(character == '='){
  }else if(character == '>'){
  }else if(character == '?'){
  }else if(character == '@'){
  }else if(character == '['){
  }else if(character == '\\'){
  }else if(character == ']'){
  }else if(character == '^'){
  }else if(character == '_'){
  }else if(character == '`'){
  }else if(character == '{'){
  }else if(character == '|'){
  }else if(character == '}'){
  }else if(character == '~'){
  }else{
    isSymbolx = false;
  }

  return isSymbolx;
}
_Bool charCharacterIsBefore(wchar_t a, wchar_t b){
  double ad, bd;

  ad = a;
  bd = b;

  return ad < bd;
}
wchar_t charDecimalDigitToCharacter(double digit){
  wchar_t c;
  if(digit == 1.0){
    c = '1';
  }else if(digit == 2.0){
    c = '2';
  }else if(digit == 3.0){
    c = '3';
  }else if(digit == 4.0){
    c = '4';
  }else if(digit == 5.0){
    c = '5';
  }else if(digit == 6.0){
    c = '6';
  }else if(digit == 7.0){
    c = '7';
  }else if(digit == 8.0){
    c = '8';
  }else if(digit == 9.0){
    c = '9';
  }else{
    c = '0';
  }
  return c;
}
double charCharacterToDecimalDigit(wchar_t c){
  double digit;

  if(c == '1'){
    digit = 1.0;
  }else if(c == '2'){
    digit = 2.0;
  }else if(c == '3'){
    digit = 3.0;
  }else if(c == '4'){
    digit = 4.0;
  }else if(c == '5'){
    digit = 5.0;
  }else if(c == '6'){
    digit = 6.0;
  }else if(c == '7'){
    digit = 7.0;
  }else if(c == '8'){
    digit = 8.0;
  }else if(c == '9'){
    digit = 9.0;
  }else{
    digit = 0.0;
  }

  return digit;
}
void AssertFalse(_Bool b, NumberReference *failures){
  if(b){
    failures->numberValue = failures->numberValue + 1.0;
  }
}
void AssertTrue(_Bool b, NumberReference *failures){
  if( !b ){
    failures->numberValue = failures->numberValue + 1.0;
  }
}
void AssertEquals(double a, double b, NumberReference *failures){
  if(a != b){
    failures->numberValue = failures->numberValue + 1.0;
  }
}
void AssertBooleansEqual(_Bool a, _Bool b, NumberReference *failures){
  if(a != b){
    failures->numberValue = failures->numberValue + 1.0;
  }
}
void AssertCharactersEqual(wchar_t a, wchar_t b, NumberReference *failures){
  if(a != b){
    failures->numberValue = failures->numberValue + 1.0;
  }
}
void AssertStringEquals(wchar_t *a, size_t aLength, wchar_t *b, size_t bLength, NumberReference *failures){
  if( !StringsEqual(a, aLength, b, bLength) ){
    failures->numberValue = failures->numberValue + 1.0;
  }
}
void AssertNumberArraysEqual(double *a, size_t aLength, double *b, size_t bLength, NumberReference *failures){
  double i;

  if(aLength == bLength){
    for(i = 0.0; i < aLength; i = i + 1.0){
      AssertEquals(a[(int)(i)], b[(int)(i)], failures);
    }
  }else{
    failures->numberValue = failures->numberValue + 1.0;
  }
}
void AssertBooleanArraysEqual(_Bool *a, size_t aLength, _Bool *b, size_t bLength, NumberReference *failures){
  double i;

  if(aLength == bLength){
    for(i = 0.0; i < aLength; i = i + 1.0){
      AssertBooleansEqual(a[(int)(i)], b[(int)(i)], failures);
    }
  }else{
    failures->numberValue = failures->numberValue + 1.0;
  }
}
void AssertStringArraysEqual(StringReference **a, size_t aLength, StringReference **b, size_t bLength, NumberReference *failures){
  double i;

  if(aLength == bLength){
    for(i = 0.0; i < aLength; i = i + 1.0){
      AssertStringEquals(a[(int)(i)]->string, a[(int)(i)]->stringLength, b[(int)(i)]->string, b[(int)(i)]->stringLength, failures);
    }
  }else{
    failures->numberValue = failures->numberValue + 1.0;
  }
}

void testReadWrite() {
   NumberReference *failures;
   failures = CreateNumberReference(0.0);
   Element *json;
   wchar_t *string, *string2;
   size_t stringLength, string2Length;
   StringArrayReference *errorMessages;
   ElementReference *elementReference;
   _Bool success;

   json = createExampleJSON();
   string = WriteJSON(&stringLength, json);
   DeleteElement(json);

   elementReference = (ElementReference *)malloc(sizeof(ElementReference));
   errorMessages = CreateStringArrayReferenceLengthValue(0.0, strparam(L""));
   success = ReadJSON(string, stringLength, elementReference, errorMessages);

   AssertTrue(success, failures);

   if(success){
      json = elementReference->element;
      //string2 = WriteJSON(&string2Length, json);

      //AssertEquals(stringLength, string2Length, failures);

      DeleteElement(json);
      //free(string2);
   }

   FreeStringArrayReference(errorMessages);
   free(elementReference);
   free(string);
   free(failures);
}
