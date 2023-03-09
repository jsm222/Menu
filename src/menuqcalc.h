#ifndef MENUQCALC_H
#define MENUQCALC_H
#include <QObject>
#include <QPointer>
#include <libqalculate/Calculator.h>
//derived from calculator_qalculate plugin in albertlauncher Copyright (c) 2023 Manuel Schneider
class MenuQCalc : public Calculator
{
public:
    MenuQCalc() : Calculator()
    {
          eo.auto_post_conversion = POST_CONVERSION_BEST;
          eo.structuring = STRUCTURING_SIMPLIFY;

          eo.parse_options.angle_unit = static_cast<AngleUnit>(ANGLE_UNIT_DEGREES );
          eo.parse_options.functions_enabled = true;
          eo.parse_options.limit_implicit_multiplication = true;
          eo.parse_options.parsing_mode = static_cast<ParsingMode>(PARSING_MODE_CONVENTIONAL);
          eo.parse_options.units_enabled = false;
          eo.parse_options.unknowns_enabled = false;
          po.indicate_infinite_series = true;
          po.interval_display = INTERVAL_DISPLAY_SIGNIFICANT_DIGITS;
          po.lower_case_e = true;
          po.use_unicode_signs = true;
    }
    QString getResult(QString expr) {
         return QString::fromStdString(calculate(expr.toStdString(),eo).print(po));
    }
    virtual ~MenuQCalc() {}
        EvaluationOptions eo;
        PrintOptions po;
};
#endif // MENUQCALC_H

