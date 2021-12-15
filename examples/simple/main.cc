// Copyright The OpenTelemetry Authors
// SPDX-License-Identifier: Apache-2.0

#include "opentelemetry/sdk/trace/simple_processor.h"
#include "opentelemetry/sdk/trace/tracer_provider.h"
#include "opentelemetry/trace/provider.h"

// Using an exporter that simply dumps span data to stdout.
#ifdef BAZEL_BUILD
#  include "examples/common/foo_library/foo_library.h"
#else
#  include "foo_library.h"
#endif
#include "opentelemetry/exporters/ostream/span_exporter.h"

namespace
{
opentelemetry::nostd::shared_ptr<opentelemetry::trace::Tracer> get_tracer()
{
  auto provider = opentelemetry::trace::Provider::GetTracerProvider();
  return provider->GetTracer("simple_example");
}
}  // namespace

namespace trace_api = opentelemetry::trace;
namespace trace_sdk = opentelemetry::sdk::trace;
namespace nostd     = opentelemetry::nostd;
namespace
{
void initTracer()
{
  auto exporter = std::unique_ptr<trace_sdk::SpanExporter>(
      new opentelemetry::exporter::trace::OStreamSpanExporter);
  auto processor = std::unique_ptr<trace_sdk::SpanProcessor>(
      new trace_sdk::SimpleSpanProcessor(std::move(exporter)));
  auto provider = nostd::shared_ptr<trace_api::TracerProvider>(
      new trace_sdk::TracerProvider(std::move(processor)));

  // Set the global trace provider
  trace_api::Provider::SetTracerProvider(provider);
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
