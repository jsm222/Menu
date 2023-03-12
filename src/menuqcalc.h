#ifndef MENUQCALC_H
#define MENUQCALC_H
#include <QObject>
#include <QPointer>
#include <libqalculate/Calculator.h>
using std::cout;
using std::endl;
using std::map;
using std::pair;
using std::string;
using std::vector;

// derived from calculator_qalculate plugin in albertlauncher Copyright (c) 2023 Manuel Schneider
class MenuQCalc : public Calculator
{
public:
    MenuQCalc() : Calculator()
    {
        eo.auto_post_conversion = POST_CONVERSION_BEST;
        eo.structuring = STRUCTURING_SIMPLIFY;
        eo.parse_options.angle_unit = static_cast<AngleUnit>(ANGLE_UNIT_DEGREES);
        eo.parse_options.functions_enabled = true;
        eo.parse_options.limit_implicit_multiplication = true;
        eo.parse_options.parsing_mode = static_cast<ParsingMode>(PARSING_MODE_CONVENTIONAL);
        eo.parse_options.units_enabled = true;
        eo.parse_options.unknowns_enabled = false;
        po.indicate_infinite_series = true;
        po.interval_display = INTERVAL_DISPLAY_SIGNIFICANT_DIGITS;
        po.lower_case_e = true;
        po.use_unicode_signs = true;
        // Enable unit conversion using the "to" keyword
        // so that "1 m to cm" works
        po.use_unit_prefixes = true;
        po.use_prefixes_for_currencies = true;
        po.use_prefixes_for_all_units = true;
        loadGlobalDefinitions();
        loadLocalDefinitions();
        loadGlobalCurrencies();
        loadGlobalUnits();
        loadGlobalVariables();
        loadGlobalFunctions();
        loadGlobalPrefixes();
        // Now "1 m to cm" works but "9 to hex" does not
    }

    QString getResult(QString expr, bool retErrors)
    {
        string str = unlocalizeExpression(expr.toStdString(), eo.parse_options);
        string parsed;
        int max_length = 100 - unicode_length(str);
        if (max_length < 50)
            max_length = 50;
        bool result_is_comparison = false;
        string result = calculateAndPrint(str, 100, eo, po, AUTOMATIC_FRACTION_AUTO,
                                          AUTOMATIC_APPROXIMATION_AUTO, &parsed, max_length,
                                          &result_is_comparison);

        return QString::fromStdString(result);
    }
    virtual ~MenuQCalc() { }
    EvaluationOptions eo;
    PrintOptions po;
};
#endif // MENUQCALC_H
