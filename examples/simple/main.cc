// Copyright The OpenTelemetry Authors
// SPDX-License-Identifier: Apache-2.0

#include "opentelemetry/sdk/trace/simple_processor.h"
#include "opentelemetry/sdk/trace/tracer_provider.h"
#include "opentelemetry/trace/provider.h"

// Using an exporter that simply dumps span data to stdout.
#include "foo_library/foo_library.h"

#include "opentelemetry/exporters/ostream/span_exporter.h"

namespace
{
opentelemetry::nostd::shared_ptr<opentelemetry::trace::Tracer> get_tracer()
{
  auto provider = opentelemetry::trace::Provider::GetTracerProvider();
  return provider->GetTracer("simple_example");
}
}  // namespace

namespace
{
void initTracer()
{
  auto exporter = std::unique_ptr<sdktrace::SpanExporter>(
      new opentelemetry::exporter::trace::OStreamSpanExporter);
  auto processor = std::unique_ptr<sdktrace::SpanProcessor>(
      new sdktrace::SimpleSpanProcessor(std::move(exporter)));
  auto provider = nostd::shared_ptr<opentelemetry::trace::TracerProvider>(
      new sdktrace::TracerProvider(std::move(processor)));

  // Set the global trace provider
  opentelemetry::trace::Provider::SetTracerProvider(provider);
}
}  // namespace

void f0()
{
  auto span  = get_tracer()->StartSpan("f0");
  auto scope = get_tracer()->WithActiveSpan(span);

  span->End();
}

int main()
{
  // Removing this line will leave the default noop TracerProvider in place.
  initTracer();

  f0();

  auto span  = get_tracer()->StartSpan("main");
  auto scope = get_tracer()->WithActiveSpan(span);

  foo_library();

  span->End();
}
